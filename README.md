# BluntRatio

**BluntRatio** is a timing plugin. It delays the whole track by **0 to 250 ms** so the performance sits later than tracks without the plugin. Tagline: *relax the pocket*.

Current build: **1.2.1** (`1.2.1.37`).

## What it does

- One automatable parameter: **Delay** (0.0–250.0 ms, 0.1 ms steps).
- Default is **0 ms** (slider at **Less**).
- The output is **only** the delayed signal. There is no dry/wet mix, no feedback, and no parallel delay.
- The delay is **not** reported as plugin latency, so DAW delay compensation will not cancel it. That lag is the effect.
- Delay time is converted from milliseconds using the host sample rate, so 44.1 / 48 / 88.2 / 96 kHz all keep the same time in ms.

## Formats

macOS builds:

- VST3
- Audio Unit (AU)
- Standalone app

After a successful build, CMake copies plugins to:

- VST3: `~/Library/Audio/Plug-Ins/VST3/BluntRatio.vst3`
- AU: `~/Library/Audio/Plug-Ins/Components`

## Requirements (macOS)

- Xcode Command Line Tools
- CMake >= 3.22
- Ninja

JUCE **9.0.2** is downloaded automatically by CMake.

## Build

```bash
cmake --preset macos-debug
cmake --build --preset macos-debug --parallel
```

Release:

```bash
cmake --preset macos-release
cmake --build --preset macos-release --parallel
```

VST3 only:

```bash
cmake --build build/debug --target BluntRatio_VST3 --parallel
```

Or run `./scripts/bootstrap-macos.sh` (needs Homebrew for CMake and Ninja).

Reload or rescan plugins in the DAW after each install.

## Tests

```bash
cmake --build build/debug --target TimeDelayTests --parallel
ctest --test-dir build/debug --output-on-failure
```

## Source layout

```text
BluntRatioAudioPlugin/
├── CMakeLists.txt
├── CMakePresets.json
├── README.md
├── LICENSE.md
├── SKILL.md
├── Assets/
│   ├── ui-panel.jpg
│   ├── slider-track.png
│   ├── slider-fill.png
│   └── slider-thumb.png
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

## UI

The editor uses the panel artwork plus slider parts (track, fill, ember thumb). **Less** / **More** and the version number are in the art. Click the **i** in the top-right of the panel for the in-plugin about screen.

## Licence

Copyright (c) 2026 **B19 Audio Production**. Project source is proprietary unless another licence is added later. See `LICENSE.md`.

JUCE 9 is dual-licensed (AGPLv3 or a commercial JUCE licence). A commercial JUCE licence may be required for a closed-source or commercial release.
