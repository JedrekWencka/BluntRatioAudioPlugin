# Blunt Ration VST

Starter repository for the **Blunt Ration** audio plugin.

## Current state

The plugin builds as:
- VST3
- Audio Unit (AU)
- Standalone app

The first implementation is intentionally simple: a smoothed **Gain** control from -24 dB to +24 dB. This proves the complete plugin pipeline before we add the real DSP.

## Requirements — macOS

- Xcode Command Line Tools
- Homebrew
- CMake >= 3.22
- Ninja

JUCE is fetched automatically by CMake and pinned to **9.0.2**.

## Fastest setup

From Terminal:

```bash
cd ~/Desktop/BluntRationVST
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
cmake --build build/debug --target BluntRation_VST3 --parallel
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
BluntRationVST/
├── CMakeLists.txt
├── CMakePresets.json
├── README.md
├── SKILL.md
├── Source/
│   ├── PluginProcessor.h
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h
│   └── PluginEditor.cpp
└── scripts/
    └── bootstrap-macos.sh
```

## Next milestone

Replace the proof-of-life gain stage with the first real Blunt Ration DSP block, while keeping a tiny testable signal path.

## JUCE licensing note

JUCE 9 is dual-licensed under AGPLv3 and a commercial JUCE licence. This repository currently treats the Blunt Ration source as proprietary, so before distributing a closed-source build, confirm that you have the appropriate JUCE commercial licence/terms for your use case.
