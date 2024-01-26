/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MainGui.h"

namespace fs = std::filesystem;
//==============================================================================
LstmMusicEditor::LstmMusicEditor (LstmMusicProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    mainGui = std::make_unique<MainGui>();
    
    //btnClickedCallback callback = generateBtnClicked;
    mainGui->setGenerateBtnClickedCallback(std::bind(&LstmMusicEditor::generateBtnClicked, this));
    
    mainGui->setPlayBtnClickedCallback(std::bind(&LstmMusicEditor::playBtnClicked, this));
    mainGui->setSaveBtnClickedCallback(std::bind(&LstmMusicEditor::saveBtnClicked, this));
    mainGui->setStopBtnClickedCallback(std::bind(&LstmMusicEditor::stopBtnClicked, this));
    mainGui->setChooseFolderBtnClickedCallback(std::bind(&LstmMusicEditor::chooseModelsPathClicked, this));
    mainGui->setStyleChangedCallback(std::bind(&LstmMusicEditor::styleChanged, this));
    audioProcessor.setSongIsFinishedCallback(std::bind(&LstmMusicEditor::songIsFinished, this));
    using namespace std::placeholders;
    audioProcessor.setProgressCallback(std::bind(&LstmMusicEditor::updateProgress, this, _1, _2));
    audioProcessor.setGenerationCompletedCallback(std::bind(&LstmMusicEditor::enableUI, this));
    
    addAndMakeVisible (mainGui.get());
    setSize (mainGui->getWidth(), mainGui->getHeight());
    
    updateProgress(0, 0);

    juce::AlertWindow::showMessageBoxAsync( juce::MessageBoxIconType::InfoIcon,
    TRANS("How to start"),
    TRANS("Before you start, please choose the folder with the models.\nYou can find the button in the \'parameters\' block." ));
    
    updateMusicalStyles();
    updateLstmDepths();
    updateTestSongs();
}

LstmMusicEditor::~LstmMusicEditor()
{
}

//==============================================================================
void LstmMusicEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

   g.setColour (juce::Colours::white);
   g.setFont (15.0f);
   g.drawFittedText ("v0.002", getLocalBounds(), juce::Justification::centred, 1);
}

void LstmMusicEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    /*
    float rowHeight = getHeight()/5;
    float colWidth = getWidth() / 3;
    float row = 0;
    float col = 0;


    miniPianoKbd.setBounds(0, rowHeight*row, getWidth(), rowHeight);
    row ++;
    resetButton.setBounds(0, rowHeight*row, colWidth, rowHeight);
    col ++;
    isLearningButton.setBounds(col * colWidth, rowHeight*row, colWidth, rowHeight);*/
    
    
}

void LstmMusicEditor::enableUI()
{
    mainGui->enableUI(true);
}

void LstmMusicEditor::disableUI()
{
    mainGui->enableUI(false);
}

void LstmMusicEditor::updateLstmDepths()
{
    const auto depths = audioProcessor.getLstmDepths();
    mainGui->fillDepthCmb(depths);
}

void LstmMusicEditor::updateMusicalStyles()
{
    const auto styles = audioProcessor.getMusicalStyles();
    mainGui->fillStyleCmb(styles);
}

void LstmMusicEditor::updateTestSongs()
{
    const auto songs = audioProcessor.getTestSongs();
    mainGui->fillSongsCmb(songs);
}

void LstmMusicEditor::sliderValueChanged (juce::Slider *slider)
{

}

void LstmMusicEditor::generateBtnClicked()
{
    auto res = this->audioProcessor.generateMelody(mainGui->getStyle(), mainGui->getSong(), mainGui->getLstmDepth(), mainGui->getMelodyLength());
    
    if (res)
    {
        disableUI();
    }
}

void LstmMusicEditor::playBtnClicked()
{
    this->audioProcessor.playMidi();
}

void LstmMusicEditor::stopBtnClicked()
{
    this->audioProcessor.stopPlayingMidi();
}

void LstmMusicEditor::saveBtnClicked()
{
    this->audioProcessor.saveMidi();
}

void LstmMusicEditor::chooseModelsPathClicked()
{
    const auto modelPath = askModelPath();
    if (!fs::exists(modelPath))
        return;
    
    this->audioProcessor.initModels(modelPath);
    mainGui->setModelsFolderPath(modelPath);
    
    updateLstmDepths();
    updateMusicalStyles();
    updateTestSongs();
}

void LstmMusicEditor::styleChanged()
{
    const auto currentStyle = mainGui->getStyle();
    this->audioProcessor.setCurrentStyle(currentStyle);
    updateTestSongs();
}

void LstmMusicEditor::songIsFinished()
{
    mainGui->onSongIsFinished();
}

void LstmMusicEditor::updateProgress(int numGenerated, int numTotal)
{
    mainGui->setProgress(float(numGenerated) / numTotal * 100);
    mainGui->updateProgressLabel(numGenerated, numTotal);
}

void LstmMusicEditor::buttonClicked(juce::Button* btn)
{
    /*
    if (btn == &resetButton){
        this->audioProcessor.generateMelody();
        //this->audioProcessor.openMidi();
    }
    if (btn == &isLearningButton){
        // if processor is learning, set to 'is not learning'
        if (this->audioProcessor.isLearning()){
            this->audioProcessor.stopLearning();
            // make the button off
            isLearningButton.setButtonText("Start learning");
        }
        else {
            this->audioProcessor.startLearning();
            //isLearningButton.setButtonText("Machine is learning");
            isLearningButton.setButtonText("Stop learning");
        }
    }*/
}

std::string LstmMusicEditor::askModelPath()
{
    std::string path;
    juce::FileChooser chooser("Select a folder with models...",
                              juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                              );

    if (chooser.browseForDirectory())
    {
        path = chooser.getResult().getFullPathName().toStdString();// fullPath();
    }
    return path;
}

void LstmMusicEditor::handleNoteOn(juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    juce::MidiMessage msg1 = juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
    audioProcessor.addMidi(msg1, 0);
    
}

void LstmMusicEditor::handleNoteOff(juce::MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    juce::MidiMessage msg2 = juce::MidiMessage::noteOff(midiChannel, midiNoteNumber, velocity);
    audioProcessor.addMidi(msg2, 0); 
}


