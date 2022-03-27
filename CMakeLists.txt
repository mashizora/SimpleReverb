cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(SimpleReverb VERSION 0.0.1)
set(BASE_TARGET_NAME ${PROJECT_NAME})
set(JUCE_VERSION 6.1.6)

# Include CPM.cmake (https://github.com/cpm-cmake/CPM.cmake)
# CPM.cmake is a cross-platform CMake script that adds dependency management capabilities to CMake.
include(cmake/CPM.cmake)
# Fetch JUCE from GitHub repo juce-framework/JUCE using CPM.cmake.
CPMAddPackage("gh:juce-framework/JUCE#${JUCE_VERSION}")

# Add a JUCE plugin target.
# More optional arguments can be found at "JUCE/docs/CMake API.md"
juce_add_plugin(${BASE_TARGET_NAME}
    COMPANY_NAME UMA
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Dem0

    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE

    FORMATS AU VST3 Standalone
    COPY_PLUGIN_AFTER_BUILD TRUE)

target_sources(${BASE_TARGET_NAME}
    PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
        src/CustomTheme.cpp
        src/RotarySlider.cpp)

target_compile_features(${BASE_TARGET_NAME}
    PRIVATE
        cxx_std_17)

target_compile_definitions(${BASE_TARGET_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${BASE_TARGET_NAME}
    PRIVATE
        juce::juce_dsp
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
