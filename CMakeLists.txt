cmake_minimum_required(VERSION 3.22)

set(BLUNTRATIO_VERSION 1.2.1)
set(BLUNTRATIO_BUILD_VERSION 1.2.1.37)

project(BluntRatioVST VERSION ${BLUNTRATIO_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Pin JUCE so builds remain reproducible.
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 9.0.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(BluntRatio
    COMPANY_NAME "BluntRatio"
    BUNDLE_ID "com.bluntratio.audio.bluntratio"
    PLUGIN_MANUFACTURER_CODE BlRt
    PLUGIN_CODE BR01
    VERSION ${BLUNTRATIO_VERSION}
    BUILD_VERSION ${BLUNTRATIO_BUILD_VERSION}
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "BluntRatio"
    DESCRIPTION "Delay a track by 0-250 ms to relax the pocket"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CATEGORIES Fx Delay
    AU_MAIN_TYPE kAudioUnitType_Effect
)

juce_generate_juce_header(BluntRatio)

juce_add_binary_data(BluntRatioAssets
    SOURCES
        Assets/ui-panel.jpg
        Assets/slider-track.png
        Assets/slider-fill.png
        Assets/slider-thumb.png
)

set_target_properties(BluntRatioAssets PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_sources(BluntRatio
    PRIVATE
        Source/DSP/TimeDelay.cpp
        Source/DSP/TimeDelay.h
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
)

target_compile_definitions(BluntRatio
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        BLUNTRATIO_BUILD_VERSION="${BLUNTRATIO_BUILD_VERSION}"
)

target_link_libraries(BluntRatio
    PRIVATE
        BluntRatioAssets
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

enable_testing()

add_executable(TimeDelayTests
    Tests/TimeDelayTests.cpp
    Source/DSP/TimeDelay.cpp
)

target_include_directories(TimeDelayTests PRIVATE Source)

target_compile_definitions(TimeDelayTests
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)

target_link_libraries(TimeDelayTests
    PRIVATE
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)

add_test(NAME TimeDelayTests COMMAND TimeDelayTests)
