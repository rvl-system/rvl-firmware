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

import {
  existsSync,
  readdirSync,
  readFileSync,
  writeFileSync,
  statSync,
} from "node:fs";
import { join, sep } from "node:path";
import { execSync } from "node:child_process";
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
  -l  --lint      lint the source code
  -b  --build     build the firmware before flashing the target
  -t  --test      run lib/rvl's unit tests on this computer, whatever the
                  target, stopping before flashing if any fail
  -f  --flash     flash the firmware after building the target
      --compiledb regenerate compile_commands.json, used by the linter
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
  build?: boolean;
  test?: boolean;
  flash?: boolean;
  compiledb?: boolean;
  help?: boolean;
};
let positionals: string[];
try {
  ({ values, positionals } = parseArgs({
    options: {
      lint: { type: "boolean", short: "l" },
      build: { type: "boolean", short: "b" },
      test: { type: "boolean", short: "t" },
      flash: { type: "boolean", short: "f" },
      compiledb: { type: "boolean" },
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
  ? "/dev/cu.usbserial-2120"
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

const SOURCE_FILES = [
  ...findFiles(join(import.meta.dirname, "src"), /(\.cpp|\.hpp|\.c|\.h)$/),
  ...findFiles(join(import.meta.dirname, "lib", "rvl", "src"), /(\.cpp|\.hpp|\.c|\.h)$/),
  ...findFiles(
    join(import.meta.dirname, "lib", "rvl-wifi", "src"),
    /(\.cpp|\.hpp|\.c|\.h)$/,
  ),
];

// clang-tidy resolves each file's include paths and flags from this database,
// so it has to exist before linting and be regenerated when the build changes.
// Only the root project has one; generating it costs a full build, so don't do
// that as a side effect of an unrelated command
if (
  values.compiledb ||
  (values.lint && !existsSync(join(import.meta.dirname, "compile_commands.json")))
) {
  console.log("Generating compile_commands.json\n");
  exec("platformio run -e compiledb -t compiledb");
  const commandsPath = join(import.meta.dirname, "compile_commands.json");
  const commands = readFileSync(commandsPath, "utf-8").replace(
    / -I[^\s]*?\.platformio[^\s]*?newlib/g,
    "",
  );
  writeFileSync(commandsPath, commands);
}

if (values.lint) {
  console.log(`Linting\n`);
  const guardError = SOURCE_FILES.reduce<boolean>((error, sourceFile) => {
    return checkHeaderGuard(sourceFile) || error;
  }, false);
  if (guardError) {
    process.exit(-1);
  }
  exec(`clang-tidy ${SOURCE_FILES.join(" ")}`, {
    CPATH: "",
  });
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
