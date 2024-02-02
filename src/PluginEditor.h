/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

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
                          public juce::Slider::Listener
{
public:
    LstmMusicEditor (LstmMusicProcessor&);
    ~LstmMusicEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged (juce::Slider *slider) override;
    void buttonClicked(juce::Button* btn) override;

private:
    //Methods for changing/updating UI state
    void updateLstmInputLengths();
    void updateMusicalStyles();
    void updateTestSongs();
    
    void songIsFinished();
    void updateProgress(int, int);
    void enableUI();
    void disableUI();
    
    //Handlers of the events from UI components
    void generateBtnClicked();
    void playBtnClicked();
    void playInitSongBtnClicked();
    void stopBtnClicked();
    void saveBtnClicked();
    void styleChanged();
    void openFolderClicked();
    void chooseModelsPathClicked();
    
    //Let the user select a folder with pre-trained models
    std::string askModelPath();
    
private:
    //smart pointer to MainGui component
    std::unique_ptr<MainGui> mainGui;
    
    //keep the last path chosen by user to save midi
    std::string lastSavePath;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LstmMusicProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LstmMusicEditor)
};
