cmake_minimum_required(VERSION 3.15)

find_package(nlohmann_json REQUIRED)

project(lstm_classical_music_generator VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 17)

# tests for chord detect
add_executable(chord-tests src/ChordDetectTest.cpp src/ChordDetector.cpp)


add_subdirectory(/Applications/JUCE ./JUCE)     # If you've put JUCE in a subdirectory called JUCE

juce_add_plugin(lstm_classical_music_generator
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME missOlivia                         # Specify the name of the plugin's author
    IS_SYNTH TRUE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT TRUE              # Does the plugin need midi output?
    IS_MIDI_EFFECT TRUE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD TRUE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Olka               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE ebc0                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "lstm_classical_music_generator")        # The name of the final executable, which can differ from the target name

# you can do this since we are not using torch any more
juce_generate_juce_header(lstm_classical_music_generator)

include_directories(lstm_classical_music_generator PRIVATE "./3rdparty/keras2cpp/src/")
include_directories(lstm_classical_music_generator PRIVATE "./models/")

target_sources(lstm_classical_music_generator
    PRIVATE
    src/PluginEditor.cpp
    src/PluginProcessor.cpp
    src/ChordDetector.cpp
    src/MainGui.cpp
    
    3rdparty/keras2cpp/src/utils.cc
    3rdparty/keras2cpp/src/baseLayer.cc
    3rdparty/keras2cpp/src/layers/activation.cc
    3rdparty/keras2cpp/src/layers/conv1d.cc
    3rdparty/keras2cpp/src/layers/conv2d.cc
    3rdparty/keras2cpp/src/layers/dense.cc
    3rdparty/keras2cpp/src/layers/elu.cc
    3rdparty/keras2cpp/src/layers/embedding.cc
    3rdparty/keras2cpp/src/layers/flatten.cc
    3rdparty/keras2cpp/src/layers/lstm.cc
    3rdparty/keras2cpp/src/layers/locally1d.cc
    3rdparty/keras2cpp/src/layers/locally2d.cc
    3rdparty/keras2cpp/src/layers/maxPooling2d.cc
    3rdparty/keras2cpp/src/layers/batchNormalization.cc
    3rdparty/keras2cpp/src/model.cc
    3rdparty/keras2cpp/src/tensor.cc
   )


target_compile_definitions(lstm_classical_music_generator
    PUBLIC # 
        JUCE_ALSA=1
        JUCE_DIRECTSOUND=1
        JUCE_DISABLE_CAUTIOUS_PARAMETER_ID_CHECKING=1
        #JUCE_PLUGINHOST_LADSPA=1
        #JUCE_PLUGINHOST_LV2=1
        #JUCE_PLUGINHOST_VST3=1
        JUCE_USE_OGGVORBIS=1
        JUCE_MODAL_LOOPS_PERMITTED=1
        #JUCE_VST3_HOST_CROSS_PLATFORM_UID=1
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(lstm_classical_music_generator
    PRIVATE
        juce::juce_audio_utils
        nlohmann_json::nlohmann_json
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)


