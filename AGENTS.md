# Working in rvl-firmware

Guidance for anyone, human or agent, changing code here. `lib/rvl` and
`rvl-node` are submodules with their own commits; this applies to them too.

## Building and testing

- Use `./project.ts` for everything it covers: `-b <target>` builds, `-t`
  runs the native tests, `-f` flashes, `-l` lints and `--format` formats in
  place. Targets are the `[env:...]` sections of `platformio.ini` plus
  `coordinator`, which is a separate PlatformIO project. `esp8266` is
  build-only. Build several targets with several `-b` calls.
- When `./project.ts` has no command for what you need, first ask whether the
  task can be put in terms of one it has. If it can't, stop and ask. Never run
  `pio` directly: `pio check`, for one, can't parse this code and passes
  anyway.
- Native tests are Unity suites in `test/`, run by `./project.ts -t`. They
  test `lib/rvl`; the Arduino code in `src/` isn't built for them.
- Never add `-fwrapv` or similar flags to make a test pass. `lib/rvl` must not
  depend on signed overflow wrapping, and the flag hides exactly that.
- For questions about undefined or implementation-defined behavior, don't
  compile a throwaway program with the host's clang. It's a different compiler
  on a different architecture. Reason it out, or write a native test.
- Check commands, such as lint and typecheck, never write files: no build step
  inside them, no emit, no formatting in place.
- `rvl-node` commits its `dist/`. After changing its source, run
  `npm run build` and include the regenerated `dist/` in the change.

## Code

- **Locks only where needed, and every one says why.** Take `lockState()`
  only for state written on one task and read on another, on both sides of
  that access. Every `lockState()`/`freeState()` pair gets a short comment
  naming the cross-task access it protects. Code without a lock needs no
  comment explaining its absence. `lockState()` disables interrupts on ESP32,
  so never emit an event, log, or do I/O while holding it.
- **Function names contain a verb,** usually first. Three to six words is fine
  when that's what it takes to say what the function does, including every
  outcome: `scheduleOrHoldContent`, not `request`. Predicates use `is` or
  `has`; event handlers use `on...`. Tests follow the same idea with a lighter
  hand.
- **Comments are rare and short.** Add one when a non-obvious hazard would bite
  the next reader, such as code that looks safe to reorder but isn't. Design
  rationale belongs in the change's discussion, not the code.
- **A function that completes a deliberate API surface stays** even when
  nothing calls it, like `beginUnicastWrite` beside the broadcast and
  multicast sends. Zero callers isn't grounds for deletion. A feature's
  leftovers are.
- **Editing firmware is cheap.** When weighing designs, judge behavior,
  correctness, simplicity and consistency between boards and rvl-node, not
  which repo has to change.
