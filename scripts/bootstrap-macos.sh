#!/usr/bin/env bash
set -euo pipefail

if ! xcode-select -p >/dev/null 2>&1; then
  echo "Xcode Command Line Tools are required. Starting installer..."
  xcode-select --install
  exit 1
fi

if ! command -v brew >/dev/null 2>&1; then
  echo "Homebrew is not installed. Install it from https://brew.sh, then rerun this script."
  exit 1
fi

brew list cmake >/dev/null 2>&1 || brew install cmake
brew list ninja >/dev/null 2>&1 || brew install ninja

echo
echo "Configuring BluntRatio..."
cmake --preset macos-debug

echo
echo "Building VST3/AU/Standalone..."
cmake --build --preset macos-debug --parallel

echo
echo "Done. JUCE COPY_PLUGIN_AFTER_BUILD is enabled, so plugin formats are copied to the standard user plugin locations when supported."
