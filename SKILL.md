# BluntRatio VST — Development Skill

## Goal
Build a production-quality audio effect plugin using JUCE, CMake, and modern C++.

## Stack
- C++17 or newer when justified
- JUCE 9.0.2 (pinned in CMake)
- CMake >= 3.22
- Ninja for local builds
- Primary target: VST3
- macOS convenience targets: AU and Standalone

## Architecture rules
1. Keep real-time DSP in `PluginProcessor` or dedicated DSP classes.
2. Never allocate memory, lock mutexes, perform file I/O, or log from `processBlock`.
3. Parameters that cross UI/audio threads must use JUCE parameter primitives or atomics.
4. Smooth gain/filter/time-varying DSP parameters to avoid zipper noise.
5. Keep DSP independent from UI wherever practical.
6. Plugin state must round-trip through `getStateInformation` / `setStateInformation`.
7. Add tests for non-trivial DSP before polishing UI.

## Working conventions
- One feature per focused commit.
- Build Debug after structural changes.
- Build Release before DAW testing or performance comparisons.
- Treat compiler warnings as bugs unless clearly justified.
- Avoid adding dependencies unless they solve a concrete problem.

## Initial milestone
BluntRatio is a 0–250 ms track delay with a single slider in 0.1 ms steps, defaulting to 0 ms. Keep the plugin loadable at every step; do not report the delay as host latency.

## Definition of done for DSP features
- Works at 44.1, 48, 88.2, 96 kHz.
- Works with variable block sizes.
- Mono and stereo behavior verified when applicable.
- No audio-thread allocations.
- Parameter automation is stable.
- State/preset reload works.
- No NaN/Inf propagation under expected inputs.
