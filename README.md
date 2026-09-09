# BluntRatio VST

**BluntRatio** delays a whole track by 0–250 ms (0.1 ms steps) so it sits later than tracks without the plugin. Default is 0 ms. The delay is the effect — it is not reported as host latency, so DAW delay compensation will not cancel it.

## Current state

The plugin builds as:
- VST3
- Audio Unit (AU)
- Standalone app

There is a single **Delay** slider from 0 to 250 ms in 0.1 ms steps, labeled only **less** / **more**. The editor uses the BluntRatio mockup art (blunt, smoke, branding) with a live slider on top.

## Requirements — macOS

- Xcode Command Line Tools
- Homebrew
- CMake >= 3.22
- Ninja

JUCE is fetched automatically by CMake and pinned to **9.0.2**.

## Fastest setup

From Terminal:

```bash
cd ~/Desktop/BluntRatioVST
./scripts/bootstrap-macos.sh
```

## Manual build

```bash
cmake --preset macos-debug
cmake --build --preset macos-debug --parallel
```

Release build:

```bash
cmake --preset macos-release
cmake --build --preset macos-release --parallel
```

## Build only the VST3

After configuration:

```bash
cmake --build build/debug --target BluntRatio_VST3 --parallel
```

## Where macOS VST3 plugins normally live

User-level VST3 path:

```text
~/Library/Audio/Plug-Ins/VST3
```

User-level AU path:

```text
~/Library/Audio/Plug-Ins/Components
```

`COPY_PLUGIN_AFTER_BUILD` is enabled in CMake.

## Source layout

```text
BluntRatioVST/
├── CMakeLists.txt
├── CMakePresets.json
├── README.md
├── SKILL.md
├── Assets/
│   └── bluntratio-ui.jpg
├── Source/
│   ├── DSP/
│   │   ├── TimeDelay.h
│   │   └── TimeDelay.cpp
│   ├── PluginProcessor.h
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h
│   └── PluginEditor.cpp
├── Tests/
│   └── TimeDelayTests.cpp
└── scripts/
    └── bootstrap-macos.sh
```

## Tests

After configuration:

```bash
cmake --build build/debug --target TimeDelayTests --parallel
ctest --test-dir build/debug --output-on-failure
```

## Next milestone

Confirm the slider sits on the mockup track in a DAW, then verify delay at 44.1/48/96 kHz with variable block sizes.

## JUCE licensing note

JUCE 9 is dual-licensed under AGPLv3 and a commercial JUCE licence. This repository currently treats the BluntRatio source as proprietary, so before distributing a closed-source build, confirm that you have the appropriate JUCE commercial licence/terms for your use case.
