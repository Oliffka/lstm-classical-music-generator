/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include <functional>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainGui  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::ComboBox::Listener,
                 public juce::Slider::Listener
{
public:
    //==============================================================================
    MainGui ();
    ~MainGui() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    using btnClickedCallback = std::function<void()>;
    using cmbChangedCallback = std::function<void()>;
    
    void setGenerateBtnClickedCallback(btnClickedCallback);
    void setPlayBtnClickedCallback(btnClickedCallback);
    void setPlayInitSongBtnClickedCallback(btnClickedCallback);
    void setStopBtnClickedCallback(btnClickedCallback);
    void setSaveBtnClickedCallback(btnClickedCallback);
    void setStyleChangedCallback(cmbChangedCallback);
    void setChooseFolderBtnClickedCallback(btnClickedCallback);
    void setOpenFolderClickedCallback(btnClickedCallback);

    void onSongIsFinished();
    
    int getLstmDepth();
    int getMelodyLength();
    std::string getStyle();
    std::string getSong();
    
    void setProgress(double);
    void updateProgressLabel(int numGenerated, int numTotal);
    void setModelsFolderPath(const std::string&);
    
    void fillStyleCmb(const std::vector<std::string>& styles);
    void fillDepthCmb(const std::vector<int>& depths);
    void fillSongsCmb(const std::vector<std::string>& songs);
    void enableUI(bool);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    juce::ColourGradient silverGradient;
    juce::Colour btnColor{0xff454444};
    juce::Colour btnTextColor{0xffFACA8D};
    juce::Colour cmbColor{0xff454444};

    btnClickedCallback generateBtnClickedCallback;
    btnClickedCallback saveBtnClickedCallback;
    btnClickedCallback playBtnClickedCallback;
    btnClickedCallback playInitSongBtnClickedCallback;
    btnClickedCallback stopBtnClickedCallback;
    btnClickedCallback openFolderClickedCallback;
    btnClickedCallback chooseFolderClickedCallback;
    
    cmbChangedCallback styleChangedCallback;
    double progress = 0.0;
    
    bool isPlaying{false};
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnGenerate;
    std::unique_ptr<juce::TextButton> btnPlay;
    std::unique_ptr<juce::ComboBox> cmbStyle;
    std::unique_ptr<juce::ComboBox> cmbSongs;
    std::unique_ptr<juce::Label> lblStyle;
    std::unique_ptr<juce::Label> lblSong;
    std::unique_ptr<juce::GroupComponent> groupParams;
    std::unique_ptr<juce::Slider> sliderLen;
    std::unique_ptr<juce::Label> lblSongLen;
    std::unique_ptr<juce::ComboBox> cmbDepth;
    std::unique_ptr<juce::Label> lblDepth;
    std::unique_ptr<juce::TextButton> btnSave;
    std::unique_ptr<juce::TextButton> btnPlayStyleSong;
    std::unique_ptr<juce::Label> lblStyleDescription;
    std::unique_ptr<juce::Label> lblModelsFolder;
    std::unique_ptr<juce::TextEditor> txtPath;
    std::unique_ptr<juce::TextButton> btnChoosePath;
    std::unique_ptr<juce::TextButton> btnOpenFolder;
    std::unique_ptr<juce::Label> lblProgress;
    std::unique_ptr<juce::ProgressBar> barProgress;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainGui)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

