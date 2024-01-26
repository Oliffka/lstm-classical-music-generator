/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

//#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>

class MainGui;
//==============================================================================
/**
*/
class LstmMusicEditor  :   public juce::AudioProcessorEditor,
                          // listen to buttons
                          public juce::Button::Listener, 
                          // listen to sliders
                          public juce::Slider::Listener,
                          // listen to piano keyboard widget
                          private juce::MidiKeyboardState::Listener

{
public:
    LstmMusicEditor (LstmMusicProcessor&);
    ~LstmMusicEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged (juce::Slider *slider) override;
    void buttonClicked(juce::Button* btn) override;
    
    // from MidiKeyboardState
    void handleNoteOn(juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float
 velocity) override; 
     // from MidiKeyboardState
    void handleNoteOff(juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override; 


private:
    void updateLstmDepths();
    void updateMusicalStyles();
    void updateTestSongs();
    
    void songIsFinished();
    void updateProgress(int, int);
    void enableUI();
    void disableUI();
    
    void generateBtnClicked();
    void playBtnClicked();
    void stopBtnClicked();
    void saveBtnClicked();
    void styleChanged();
    void openFolderClicked();
    void chooseModelsPathClicked();
    std::string askModelPath();
    
private:
    std::unique_ptr<MainGui> mainGui;
    std::function<void(void*)> voidFunction;
    std::string lastSavePath;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LstmMusicProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LstmMusicEditor)
};
