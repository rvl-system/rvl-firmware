#!/usr/bin/env node
/*
Copyright (c) Bryan Hughes <bryan@nebri.us>

This file is part of RVL Firmware.


RVL Firmware is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RVL Firmware is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RVL Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/

import { existsSync, readdirSync, readFileSync, statSync } from "node:fs";
import { basename, join, sep } from "node:path";
import { execFileSync, execSync, spawnSync } from "node:child_process";
import { parseArgs } from "node:util";

function showHelp(): void {
  console.log(
    `Usage: ./project.ts [OPTIONS].. [TARGET]

Builds and flashes the firmware supplied by TARGET. TARGET is the name of an
[env:TARGET] section of platformio.ini, or "coordinator" for the transport
coordinator, which is a separate PlatformIO project under coordinator/. The
"esp8266" target is build-only: it compiles the ESP8266 transport, which no
board here runs.

OPTIONS:
  -l  --lint      check header guards, formatting and clang-tidy's checks on
                  all the source, whatever the target, changing no files
      --format    format the source code in place
  -b  --build     build the firmware before flashing the target
  -t  --test      run lib/rvl's unit tests on this computer, whatever the
                  target, stopping before flashing if any fail
  -f  --flash     flash the firmware after building the target
      --help      display this help and exit
`,
  );
}

function error(message: string): never {
  console.error(`Error: ${message}\n`);
  showHelp();
  process.exit(-1);
}

let values: {
  lint?: boolean;
  format?: boolean;
  build?: boolean;
  test?: boolean;
  flash?: boolean;
  help?: boolean;
};
let positionals: string[];
try {
  ({ values, positionals } = parseArgs({
    options: {
      lint: { type: "boolean", short: "l" },
      format: { type: "boolean" },
      build: { type: "boolean", short: "b" },
      test: { type: "boolean", short: "t" },
      flash: { type: "boolean", short: "f" },
      help: { type: "boolean" },
    },
    allowPositionals: true,
  }));
} catch (e) {
  // parseArgs rejects unknown options, which is what catches a mistyped flag
  // instead of silently treating it as the target name
  error((e as Error).message);
}

if (values.help) {
  showHelp();
  process.exit(0);
}

if (positionals.length > 1) {
  error(`expected at most one TARGET, got ${positionals.length}.`);
}
const target = positionals[0] ?? "controller";

// The coordinator differs from the boards in two ways: it is its own
// PlatformIO project rather than another env of this one, and it is flashed
// through a dev kit's onboard serial device rather than the FTDI cable the
// boards use. Resolving both here keeps every command below target-agnostic.
const isCoordinator = target === "coordinator";
const projectDir = isCoordinator
  ? join(import.meta.dirname, "coordinator")
  : import.meta.dirname;
const serialPort = isCoordinator
  ? "/dev/cu.usbserial-212220"
  : "/dev/tty.usbserial-FTAV921H";
const targetUrl = join(projectDir, ".pio", "build", target, "firmware.bin");

function exec(command: string, env?: NodeJS.ProcessEnv, cwd?: string): void {
  try {
    execSync(command, {
      stdio: "inherit",
      cwd: cwd ?? import.meta.dirname,
      // Extend rather than replace: passing a bare object to execSync drops
      // PATH and everything else the child needs
      env: { ...process.env, ...env },
    });
  } catch {
    process.exit(-1);
  }
}

function findFiles(dir: string, pattern: RegExp): string[] {
  const files = readdirSync(dir);
  const foundFiles: string[] = [];
  for (const file of files) {
    const filePath = join(dir, file);
    if (statSync(filePath).isDirectory()) {
      foundFiles.push(...findFiles(filePath, pattern));
    } else if (filePath.match(pattern)) {
      foundFiles.push(filePath);
    }
  }
  return foundFiles;
}

function checkHeaderGuard(file: string): boolean {
  if (!file.endsWith(".hpp") && !file.endsWith(".h")) {
    return false;
  }
  const contents = readFileSync(file, "utf-8").split("\n");
  let ifdef: string | undefined;
  let def: string | undefined;
  const splitFile = file.replace(/-/g, "_").toUpperCase().split(sep);
  const headerGuardSegment = splitFile.slice(splitFile.lastIndexOf("SRC") + 1);
  const filename = headerGuardSegment
    .pop()!
    .replace(".HPP", "")
    .replace(".H", "");
  headerGuardSegment.push(...filename.split("_"), "H_");
  for (let line of contents) {
    line = line.trimEnd();
    if (ifdef && def) {
      break;
    }
    if (line.startsWith("#ifndef") && !ifdef) {
      ifdef = line.split(" ")[1];
    }
    if (line.startsWith("#define") && !def) {
      def = line.split(" ")[1];
    }
  }
  const expected = headerGuardSegment.join("_");
  if (expected !== ifdef) {
    console.error(
      `Invalid #ifdef header guard ${ifdef} in ${file}. Expected ${expected}`,
    );
    return true;
  }
  if (expected !== def) {
    console.error(
      `Invalid #define header guard ${def} in ${file}. Expected ${expected}`,
    );
    return true;
  }
  return false;
}

// The tests are formatted but not linted: Unity's assertion macros would bury
// them in findings
const FORMATTED_DIRS = [
  "src",
  "lib/rvl/src",
  "lib/rvl-esp32-wifi/src",
  "lib/rvl-wifi/src",
  "coordinator/src",
  "test",
];
const LINTED_DIRS = FORMATTED_DIRS.filter((dir) => dir !== "test");

function findSourceFiles(dirs: string[]): string[] {
  return dirs.flatMap((dir) =>
    findFiles(join(import.meta.dirname, dir), /\.(cpp|hpp)$/),
  );
}

// Imported here rather than at the top, so building and flashing don't need
// npm install. clang-tidy must be 22 or later: earlier versions analyze the
// library code too, only to drop its findings, which multiplies the run time
async function loadClangTools() {
  try {
    return await import("@polycam/clang-tools");
  } catch {
    error(`clang-format and clang-tidy are missing, run "npm install".`);
  }
}

interface BuildMetadata {
  defines: string[];
  includes: { build: string[]; compatlib: string[] };
  cxx_flags: string[];
  cxx_path: string;
}

// The controller's flags parse every linted file, including the coordinator
// and rvl-wifi, which takes its ESP32 branch
function readControllerMetadata(): BuildMetadata {
  try {
    const output = execFileSync(
      "platformio",
      ["project", "metadata", "-e", "controller", "--json-output"],
      {
        cwd: import.meta.dirname,
        encoding: "utf-8",
        stdio: ["ignore", "pipe", "inherit"],
      },
    );
    return JSON.parse(output.trim().split("\n").at(-1)!).controller;
  } catch {
    error("couldn't read the controller's build metadata.");
  }
}

// pio check can't be used: it parses for this computer instead of the chip,
// and with system includes the cross compiler doesn't use. So clang gets the
// compiler's target, and its system includes in its own search order. Every
// include outside our own source is a system include, which clang-tidy skips
function buildCompileArgs(metadata: BuildMetadata): string[] {
  const ownDirs = LINTED_DIRS.map((dir) => join(import.meta.dirname, dir));
  const isOwn = (include: string) =>
    ownDirs.some((dir) => include === dir || include.startsWith(dir + sep));
  const std = metadata.cxx_flags
    .filter((flag) => flag.startsWith("-std="))
    .at(-1);
  if (!std) {
    error("the controller's build flags name no C++ standard.");
  }
  const systemIncludes = spawnSync(
    metadata.cxx_path,
    ["-x", "c++", std, "-E", "-v", "-"],
    { input: "", encoding: "utf-8" },
  )
    .stderr.split("#include <...> search starts here:")[1]
    ?.split("End of search list.")[0]
    .split("\n")
    .map((line) => line.trim())
    .filter(Boolean);
  if (!systemIncludes) {
    error(`couldn't read the system includes of ${metadata.cxx_path}.`);
  }
  return [
    `--target=${basename(metadata.cxx_path).replace(/-g\+\+$/, "")}`,
    std,
    ...metadata.defines.map((define) => `-D${define}`),
    ...[...metadata.includes.build, ...metadata.includes.compatlib].flatMap(
      (include) => (isOwn(include) ? [`-I${include}`] : ["-isystem", include]),
    ),
    "-nostdlibinc",
    ...systemIncludes.flatMap((include) => ["-isystem", include]),
  ];
}

if (values.format) {
  console.log("Formatting\n");
  const { run } = await loadClangTools();
  const files = findSourceFiles(FORMATTED_DIRS);
  if (run("clang-format", ["-i", ...files]).status !== 0) {
    process.exit(-1);
  }
}

if (values.lint) {
  console.log("Linting\n");
  const { run } = await loadClangTools();
  const guardsFailed = findSourceFiles(LINTED_DIRS).reduce<boolean>(
    (failed, file) => checkHeaderGuard(file) || failed,
    false,
  );
  const formatted =
    run("clang-format", [
      "--dry-run",
      "-Werror",
      ...findSourceFiles(FORMATTED_DIRS),
    ]).status === 0;
  // Every header is linted as its own translation unit, so its findings are
  // reported once rather than once per file that includes it
  const tidied =
    run("clang-tidy", [
      "--quiet",
      "--warnings-as-errors=*",
      "--header-filter=^$",
      ...findSourceFiles(LINTED_DIRS),
      "--",
      ...buildCompileArgs(readControllerMetadata()),
    ]).status === 0;
  if (guardsFailed || !formatted || !tidied) {
    process.exit(-1);
  }
}

if (values.build) {
  console.log(`Building ${target}\n`);
  exec(`platformio run -e ${target}`, undefined, projectDir);
}

// Always from the root project, since the tests exercise lib/rvl rather than
// any one target
if (values.test) {
  console.log("Testing\n");
  exec("platformio test -e native");
}

if (values.flash) {
  console.log(`Flashing ${target}\n`);
  // The esp8266 target exists to compile the ESP8266 transport, which is a
  // submodule shared with other projects. No board here runs it
  if (target === "esp8266") {
    error(`target "esp8266" is build-only, there is no board to flash.\n`);
  }
  if (!existsSync(targetUrl)) {
    error(`unknown or unbuilt target "${target}".\n`);
  }
  exec(
    `esptool --port ${serialPort} --baud 460800 write-flash -z 0x10000 ${targetUrl}`,
  );
}
