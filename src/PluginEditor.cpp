/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MainGui.h"
#include <filesystem>

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
    mainGui->setPlayInitSongBtnClickedCallback(std::bind(&LstmMusicEditor::playInitSongBtnClicked, this));
    mainGui->setSaveBtnClickedCallback(std::bind(&LstmMusicEditor::saveBtnClicked, this));
    mainGui->setStopBtnClickedCallback(std::bind(&LstmMusicEditor::stopBtnClicked, this));
    mainGui->setOpenFolderClickedCallback(std::bind(&LstmMusicEditor::openFolderClicked, this));
    mainGui->setChooseFolderBtnClickedCallback(std::bind(&LstmMusicEditor::chooseModelsPathClicked, this));
    mainGui->setStyleChangedCallback(std::bind(&LstmMusicEditor::styleChanged, this));
    audioProcessor.setSongIsFinishedCallback(std::bind(&LstmMusicEditor::songIsFinished, this));
    using namespace std::placeholders;
    audioProcessor.setProgressCallback(std::bind(&LstmMusicEditor::updateProgress, this, _1, _2));
    audioProcessor.setGenerationCompletedCallback(std::bind(&LstmMusicEditor::enableUI, this));
    
    addAndMakeVisible (mainGui.get());
    setSize (mainGui->getWidth(), mainGui->getHeight());
    
    updateProgress(0, 0);

    if (!audioProcessor.modelsAreLoaded)
    {
        juce::AlertWindow::showMessageBoxAsync( juce::MessageBoxIconType::InfoIcon,
                                               TRANS("How to start"),
                                               TRANS("Before you start, please choose the folder with the models.\nYou can find the button in the \'parameters\' block." ));
    }
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
        updateProgress(0, 0);
        disableUI();
    }
}

void LstmMusicEditor::playBtnClicked()
{
    this->audioProcessor.playGeneratedSong();
}

void LstmMusicEditor::playInitSongBtnClicked()
{
    this->audioProcessor.playInitSong(mainGui->getStyle(), mainGui->getSong());
}

void LstmMusicEditor::stopBtnClicked()
{
    this->audioProcessor.stopPlayingMidi();
}

void LstmMusicEditor::openFolderClicked()
{
    juce::File midiFile{lastSavePath};
    midiFile.revealToUser();
}

void LstmMusicEditor::saveBtnClicked()
{
    std::string midiPath;
    juce::FileChooser chooser("Save your midi as...",
            juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
            "*.mid");
    
    if (chooser.browseForFileToSave(true))
    {
        midiPath = chooser.getResult().getFullPathName().toStdString();
        this->audioProcessor.saveMidi(midiPath);
        lastSavePath = midiPath;
    }

}

void LstmMusicEditor::chooseModelsPathClicked()
{
    const auto modelPath = askModelPath();
    if (!fs::exists(fs::path{modelPath}))
        return;
    
    this->audioProcessor.initModels(modelPath);
    mainGui->setModelsFolderPath(modelPath);
    
    updateMusicalStyles();
    styleChanged();
}

void LstmMusicEditor::styleChanged()
{
    const auto currentStyle = mainGui->getStyle();
    this->audioProcessor.setCurrentStyle(currentStyle);
    updateLstmDepths();
    updateTestSongs();
}

void LstmMusicEditor::songIsFinished()
{
    mainGui->onSongIsFinished();
}

void LstmMusicEditor::updateProgress(int numGenerated, int numTotal)
{
    if (numTotal == 0)
        mainGui->setProgress(0);
    else
        mainGui->setProgress(double(numGenerated) / numTotal);
    
    mainGui->updateProgressLabel(numGenerated, numTotal);
}

void LstmMusicEditor::buttonClicked(juce::Button* btn)
{
}

std::string LstmMusicEditor::askModelPath()
{
    std::string path;
    juce::FileChooser chooser("Select a folder with models...",
                              juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                              );

    if (chooser.browseForDirectory())
    {
        path = chooser.getResult().getFullPathName().toStdString();
    }
    return path;
}
