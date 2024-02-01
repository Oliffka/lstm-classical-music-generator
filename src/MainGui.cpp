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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MainGui.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainGui::MainGui ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]
    btnGenerate.reset (new juce::TextButton ("btnGenerate"));
    addAndMakeVisible (btnGenerate.get());
    btnGenerate->setButtonText (TRANS("Generate"));
    btnGenerate->addListener (this);

    btnGenerate->setBounds (32, 406, 104, 40);

    btnPlay.reset (new juce::TextButton ("btnPlay"));
    addAndMakeVisible (btnPlay.get());
    btnPlay->setTooltip (TRANS("play newly generated musical piece\n"));
    btnPlay->setButtonText (TRANS("Play"));
    btnPlay->addListener (this);

    btnPlay->setBounds (152, 407, 72, 40);

    cmbStyle.reset (new juce::ComboBox ("cmbStyle"));
    addAndMakeVisible (cmbStyle.get());
    cmbStyle->setEditableText (false);
    cmbStyle->setJustificationType (juce::Justification::centredLeft);
    cmbStyle->setTextWhenNothingSelected (juce::String());
    cmbStyle->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cmbStyle->addListener (this);

    cmbStyle->setBounds (32, 72, 152, 24);

    cmbSongs.reset (new juce::ComboBox ("cmbSongs"));
    addAndMakeVisible (cmbSongs.get());
    cmbSongs->setEditableText (false);
    cmbSongs->setJustificationType (juce::Justification::centredLeft);
    cmbSongs->setTextWhenNothingSelected (juce::String());
    cmbSongs->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cmbSongs->addListener (this);

    cmbSongs->setBounds (33, 144, 295, 24);

    lblStyle.reset (new juce::Label ("lblStyle",
                                     TRANS("Choose musician:")));
    addAndMakeVisible (lblStyle.get());
    lblStyle->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblStyle->setJustificationType (juce::Justification::centredLeft);
    lblStyle->setEditable (false, false, false);
    lblStyle->setColour (juce::Label::textColourId, juce::Colour (0xff525351));
    lblStyle->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblStyle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblStyle->setBounds (27, 40, 150, 24);

    lblSong.reset (new juce::Label ("lblSong",
                                    TRANS("Choose song:")));
    addAndMakeVisible (lblSong.get());
    lblSong->setTooltip (TRANS("choose the song from predefined list to be used as a seed for music generation"));
    lblSong->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblSong->setJustificationType (juce::Justification::centredLeft);
    lblSong->setEditable (false, false, false);
    lblSong->setColour (juce::Label::textColourId, juce::Colour (0xff525351));
    lblSong->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblSong->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblSong->setBounds (28, 112, 150, 24);

    groupParams.reset (new juce::GroupComponent ("groupParams",
                                                 TRANS("Parameters")));
    addAndMakeVisible (groupParams.get());

    groupParams->setBounds (32, 192, 416, 192);

    sliderLen.reset (new juce::Slider ("sliderLen"));
    addAndMakeVisible (sliderLen.get());
    sliderLen->setRange (10, 200, 1);
    sliderLen->setSliderStyle (juce::Slider::LinearBar);
    sliderLen->setTextBoxStyle (juce::Slider::TextBoxLeft, true, 80, 20);
    sliderLen->addListener (this);

    sliderLen->setBounds (190, 334, 239, 24);

    lblSongLen.reset (new juce::Label ("lblSongLen",
                                       TRANS("How many notes to generate:\n")));
    addAndMakeVisible (lblSongLen.get());
    lblSongLen->setTooltip (TRANS("Choose the number of notes/chords to generate"));
    lblSongLen->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblSongLen->setJustificationType (juce::Justification::centredLeft);
    lblSongLen->setEditable (false, false, false);
    lblSongLen->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblSongLen->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblSongLen->setBounds (188, 294, 266, 32);

    cmbDepth.reset (new juce::ComboBox ("cmbDepth"));
    addAndMakeVisible (cmbDepth.get());
    cmbDepth->setTooltip (TRANS("choose the input length of the pretrained LSTM model"));
    cmbDepth->setEditableText (false);
    cmbDepth->setJustificationType (juce::Justification::centredLeft);
    cmbDepth->setTextWhenNothingSelected (juce::String());
    cmbDepth->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cmbDepth->addItem (TRANS("15"), 1);
    cmbDepth->addItem (TRANS("50"), 2);
    cmbDepth->addItem (TRANS("100"), 3);
    cmbDepth->addListener (this);

    cmbDepth->setBounds (48, 334, 130, 24);

    lblDepth.reset (new juce::Label ("lblDepth",
                                     TRANS("LSTM input length:")));
    addAndMakeVisible (lblDepth.get());
    lblDepth->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblDepth->setJustificationType (juce::Justification::centredLeft);
    lblDepth->setEditable (false, false, false);
    lblDepth->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblDepth->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblDepth->setBounds (48, 294, 130, 32);

    btnSave.reset (new juce::TextButton ("btnSave"));
    addAndMakeVisible (btnSave.get());
    btnSave->setButtonText (TRANS("Save..."));
    btnSave->addListener (this);

    btnSave->setBounds (240, 408, 80, 40);

    barProgress.reset(new juce::ProgressBar (progress));
    addAndMakeVisible (barProgress.get());
    barProgress->setBounds (33, 507, 415, 24);
                      
    btnPlayStyleSong.reset (new juce::TextButton ("btnPlayStyleSong"));
    addAndMakeVisible (btnPlayStyleSong.get());
    btnPlayStyleSong->setTooltip (TRANS("play newly generated musical piece\n"));
    btnPlayStyleSong->setButtonText (TRANS("Play"));
    btnPlayStyleSong->addListener (this);

    btnPlayStyleSong->setBounds (342, 136, 104, 40);

    lblStyleDescription.reset (new juce::Label ("lblStyleDescription",
                                                TRANS("Which composer\'s style do you want to use for your new song? Choose composer and initial musical piece\n"
                                                "\n")));
    addAndMakeVisible (lblStyleDescription.get());
    lblStyleDescription->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblStyleDescription->setJustificationType (juce::Justification::centredLeft);
    lblStyleDescription->setEditable (false, false, false);
    lblStyleDescription->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblStyleDescription->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblStyleDescription->setBounds (208, 60, 240, 48);

    lblModelsFolder.reset (new juce::Label ("new label",
                                            TRANS("Path to models:")));
    addAndMakeVisible (lblModelsFolder.get());
    lblModelsFolder->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblModelsFolder->setJustificationType (juce::Justification::centredLeft);
    lblModelsFolder->setEditable (false, false, false);
    lblModelsFolder->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblModelsFolder->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblModelsFolder->setBounds (46, 216, 146, 24);

    txtPath.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (txtPath.get());
    txtPath->setMultiLine (false);
    txtPath->setReturnKeyStartsNewLine (false);
    txtPath->setReadOnly (false);
    txtPath->setScrollbarsShown (true);
    txtPath->setCaretVisible (true);
    txtPath->setPopupMenuEnabled (true);
    txtPath->setText (juce::String());

    txtPath->setBounds (48, 248, 320, 24);

    btnChoosePath.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (btnChoosePath.get());
    btnChoosePath->setButtonText (TRANS("..."));
    btnChoosePath->addListener (this);

    btnChoosePath->setBounds (380, 248, 48, 25);

    btnOpenFolder.reset (new juce::TextButton ("btnOpenFolder"));
    addAndMakeVisible (btnOpenFolder.get());
    btnOpenFolder->setButtonText (TRANS("Open folder..."));
    btnOpenFolder->addListener (this);

    btnOpenFolder->setBounds (336, 408, 112, 40);
    
    lblProgress.reset (new juce::Label ("new label",
                                            TRANS("Progress: 10 of 50 notes generated\n")));
    addAndMakeVisible (lblProgress.get());
    lblProgress->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblProgress->setJustificationType (juce::Justification::centredLeft);
    lblProgress->setEditable (false, false, false);
    lblProgress->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblProgress->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblProgress->setBounds (30, 472, 264, 24);

    //[UserPreSize]
    btnGenerate->setColour(juce::TextButton::buttonColourId, btnColor);
    btnGenerate->setColour(juce::TextButton::textColourOffId, btnTextColor);

    btnPlay->setColour(juce::TextButton::buttonColourId, btnColor);
    btnPlay->setColour(juce::TextButton::textColourOffId, btnTextColor);
    
    btnPlayStyleSong->setColour(juce::TextButton::buttonColourId, btnColor);
    btnPlayStyleSong->setColour(juce::TextButton::textColourOffId, btnTextColor);

    btnSave->setColour(juce::TextButton::textColourOffId, btnTextColor);
    btnSave->setColour(juce::TextButton::buttonColourId, btnColor);
    
    btnChoosePath->setColour(juce::TextButton::textColourOffId, btnTextColor);
    btnChoosePath->setColour(juce::TextButton::buttonColourId, btnColor);
    
    txtPath->setColour(juce::TextEditor::textColourId, btnTextColor);
    txtPath->setColour(juce::TextEditor::backgroundColourId, btnColor);
    
    btnOpenFolder->setColour(juce::TextButton::textColourOffId, btnTextColor);
    btnOpenFolder->setColour(juce::TextButton::buttonColourId, btnColor);

    cmbDepth->setColour(juce::ComboBox::backgroundColourId, cmbColor);
    cmbSongs->setColour(juce::ComboBox::backgroundColourId, cmbColor);
    cmbStyle->setColour(juce::ComboBox::backgroundColourId, cmbColor);

    cmbDepth->setColour(juce::ComboBox::outlineColourId, btnTextColor);
    cmbSongs->setColour(juce::ComboBox::outlineColourId, btnTextColor);
    cmbStyle->setColour(juce::ComboBox::outlineColourId, btnTextColor);

    groupParams->setColour(juce::GroupComponent::outlineColourId, btnTextColor);
    groupParams->setColour(juce::GroupComponent::textColourId, btnColor);

    lblDepth->setColour(juce::Label::textColourId, btnColor);
    lblSongLen->setColour(juce::Label::textColourId, btnColor);
    lblSong->setColour(juce::Label::textColourId, btnColor);
    lblStyle->setColour(juce::Label::textColourId, btnColor);
    lblModelsFolder->setColour(juce::Label::textColourId, btnColor);
    lblStyleDescription->setColour(juce::Label::textColourId, btnColor);
    lblProgress->setColour(juce::Label::textColourId, btnTextColor);

    sliderLen->setColour(juce::Slider::textBoxOutlineColourId, btnTextColor);
    sliderLen->setColour(juce::Slider::trackColourId, btnColor);
    sliderLen->setColour(juce::Slider::textBoxTextColourId, btnTextColor);
            
    barProgress->setColour(juce::ProgressBar::backgroundColourId, btnColor);
    barProgress->setColour(juce::ProgressBar::foregroundColourId, btnTextColor);

    setSize (480, 580);
    //[/UserPreSize]

    //[Constructor] You can add your own custom stuff here..
    txtPath->setEnabled(false);
    sliderLen->setValue(50);
    //[/Constructor]
}

MainGui::~MainGui()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    btnGenerate = nullptr;
    btnPlay = nullptr;
    cmbStyle = nullptr;
    cmbSongs = nullptr;
    lblStyle = nullptr;
    lblSong = nullptr;
    groupParams = nullptr;
    sliderLen = nullptr;
    lblSongLen = nullptr;
    cmbDepth = nullptr;
    lblDepth = nullptr;
    btnSave = nullptr;
    btnPlayStyleSong = nullptr;
    lblStyleDescription = nullptr;
    lblModelsFolder = nullptr;
    txtPath = nullptr;
    btnChoosePath = nullptr;
    btnOpenFolder = nullptr;
    lblProgress = nullptr;
    barProgress = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainGui::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    juce::Colour mint = juce::Colour(0xff39F8FB);
    juce::Colour turquoise = juce::Colour(0xff043F3F);

    juce::ColourGradient cg = juce::ColourGradient::vertical(mint, 0.0, turquoise, getHeight());
    g.setGradientFill(cg);
    g.fillAll();
    //[/UserPaint]
}

void MainGui::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainGui::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnGenerate.get())
    {
        //[UserButtonCode_btnGenerate] -- add your button handler code here..
        if (generateBtnClickedCallback)
            generateBtnClickedCallback();
        //[/UserButtonCode_btnGenerate]
    }
    else if (buttonThatWasClicked == btnPlay.get())
    {
        //[UserButtonCode_btnPlay] -- add your button handler code here..
        if (!isPlaying)
        {
            if (playBtnClickedCallback)
                playBtnClickedCallback();
            btnPlay->setTooltip (TRANS("stop playing newly generated musical piece"));
            btnPlay->setButtonText (TRANS("Stop"));
            btnPlayStyleSong->setEnabled(false);
        }
        else
        {
            if (stopBtnClickedCallback)
                stopBtnClickedCallback();
            btnPlay->setTooltip (TRANS("play newly generated musical piece"));
            btnPlay->setButtonText (TRANS("Play"));
            btnPlayStyleSong->setEnabled(true);
        }
        isPlaying = !isPlaying;
        //[/UserButtonCode_btnPlay]
    }
    else if (buttonThatWasClicked == btnSave.get())
    {
        //[UserButtonCode_btnSave] -- add your button handler code here..
        if (saveBtnClickedCallback)
            saveBtnClickedCallback();
        //[/UserButtonCode_btnSave]
    }
    else if (buttonThatWasClicked == btnPlayStyleSong.get())
    {
        //[UserButtonCode_btnPlayStyleSong] -- add your button handler code here..
        if (!isPlaying)
        {
            if (playInitSongBtnClickedCallback)
                playInitSongBtnClickedCallback();
            btnPlayStyleSong->setTooltip (TRANS("stop playing musical piece\n"));
            btnPlayStyleSong->setButtonText (TRANS("Stop"));
            btnPlay->setEnabled(false);
        }
        else
        {
            if (stopBtnClickedCallback)
                stopBtnClickedCallback();
            btnPlayStyleSong->setTooltip (TRANS("play musical piece\n"));
            btnPlayStyleSong->setButtonText (TRANS("Play"));
            btnPlay->setEnabled(true);
        }
        isPlaying = !isPlaying;
        //[/UserButtonCode_btnPlayStyleSong]
    }
    else if (buttonThatWasClicked == btnChoosePath.get())
    {
        //[UserButtonCode_btnChoosePath] -- add your button handler code here..
        if (chooseFolderClickedCallback)
            chooseFolderClickedCallback();
        //[/UserButtonCode_btnChoosePath]
    }
    else if (buttonThatWasClicked == btnOpenFolder.get())
    {
        //[UserButtonCode_btnOpenFolder] -- add your button handler code here..
        if (openFolderClickedCallback)
            openFolderClickedCallback();
        //[/UserButtonCode_btnOpenFolder]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void MainGui::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cmbStyle.get())
    {
        //[UserComboBoxCode_cmbStyle] -- add your combo box handling code here..
        if (styleChangedCallback)
        {
            styleChangedCallback();
        }
        //[/UserComboBoxCode_cmbStyle]
    }
    else if (comboBoxThatHasChanged == cmbSongs.get())
    {
        //[UserComboBoxCode_cmbSongs] -- add your combo box handling code here..
        //[/UserComboBoxCode_cmbSongs]
    }
    else if (comboBoxThatHasChanged == cmbDepth.get())
    {
        //[UserComboBoxCode_cmbDepth] -- add your combo box handling code here..
        //[/UserComboBoxCode_cmbDepth]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MainGui::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sliderLen.get())
    {
        //[UserSliderCode_sliderLen] -- add your slider handling code here..
        //[/UserSliderCode_sliderLen]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MainGui::enableUI(bool enable)
{
    MessageManager::callAsync([enable, this] ()
    {
        btnGenerate->setEnabled(enable);
        btnSave->setEnabled(enable);
        btnPlay->setEnabled(enable);
        btnPlayStyleSong->setEnabled(enable);
        btnChoosePath->setEnabled(enable);
        txtPath->setEnabled(enable);
        cmbDepth->setEnabled(enable);
        sliderLen->setEnabled(enable);
        cmbStyle->setEnabled(enable);
        cmbSongs->setEnabled(enable);
    });
}

void MainGui::setModelsFolderPath(const std::string& folderPath)
{
    this->txtPath->setText(folderPath);
}

void MainGui::setProgress(double curProgress)
{
    progress = curProgress;
}

void MainGui::updateProgressLabel(int numGenerated, int numTotal)
{
    String text;
    if (numGenerated == numTotal && numTotal != 0)
    {
        text = TRANS("Progress: ") + TRANS("completed");
    }
    else if (numGenerated >= 0 && numTotal > 0 )
    {
        text = TRANS("Progress: ") + String(numGenerated) + " / " + String(numTotal) + TRANS(" generated");
    }
    else
    {
        text = TRANS("Progress: ");
    }
    
    MessageManager::callAsync([this, text] ()
    {
        this->lblProgress->setText(text, juce::NotificationType::sendNotification);
    });
}

void MainGui::onSongIsFinished()
{
    if (isPlaying)
    {
        MessageManager::callAsync(
                  [this] () {
                      this->btnPlay->setTooltip (TRANS("play newly generated musical piece"));
                      this->btnPlay->setButtonText (TRANS("Play"));
                      
                      btnPlayStyleSong->setTooltip (TRANS("play musical piece"));
                      btnPlayStyleSong->setButtonText (TRANS("Play"));
                      
                      btnPlayStyleSong->setEnabled(true);
                      btnPlay->setEnabled(true);
                  });
                                  
        isPlaying = false;
    }
}

void MainGui::fillInputLengthCmb(const std::vector<int>& depths)
{
    cmbDepth->clear();
    int id = 1;
    for (auto depth: depths)
    {
        cmbDepth->addItem(String(depth), id);
        id++;
    }
    cmbDepth->setSelectedItemIndex(0);
}

void MainGui::fillStyleCmb(const std::vector<std::string>& styles)
{
    cmbStyle->clear();
    int id = 1;
    for (auto style: styles)
    {
        cmbStyle->addItem(String(style), id);
        id++;
    }
    cmbStyle->setSelectedItemIndex(0);
}

void MainGui::fillSongsCmb(const std::vector<std::string>& songs)
{
    cmbSongs->clear();
    int id = 1;
    for (auto song: songs)
    {
        cmbSongs->addItem(String(song), id);
        id++;
    }
    cmbSongs->setSelectedItemIndex(0);
}

int MainGui::getInputLength() const
{
    return cmbDepth->getText().getIntValue();
}

int MainGui::getMelodyLength() const
{
    int value = static_cast<int>(sliderLen->getValue());
    return value;
}

std::string MainGui::getStyle() const
{
    return cmbStyle->getText().toStdString();
}

std::string MainGui::getSong() const
{
    return cmbSongs->getText().toStdString();
}

void MainGui::setGenerateBtnClickedCallback(btnClickedCallback callback)
{
    generateBtnClickedCallback = callback;
}

void MainGui::setPlayBtnClickedCallback(btnClickedCallback callback)
{
    playBtnClickedCallback = callback;
}

void MainGui::setStopBtnClickedCallback(btnClickedCallback callback)
{
    stopBtnClickedCallback = callback;
}

void MainGui::setSaveBtnClickedCallback(btnClickedCallback callback)
{
    saveBtnClickedCallback = callback;
}

void MainGui::setStyleChangedCallback(cmbChangedCallback callback)
{
    styleChangedCallback = callback;
}

void MainGui::setChooseFolderBtnClickedCallback(btnClickedCallback callback)
{
    chooseFolderClickedCallback = callback;
}

void MainGui::setOpenFolderClickedCallback(btnClickedCallback callback)
{
    openFolderClickedCallback = callback;
}

void MainGui::setPlayInitSongBtnClickedCallback(btnClickedCallback callback)
{
    playInitSongBtnClickedCallback = callback;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainGui" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTBUTTON name="btnGenerate" id="99176a9448c0e159" memberName="btnGenerate"
              virtualName="" explicitFocusOrder="0" pos="32 406 104 40" buttonText="Generate"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnPlay" id="9534f7fdf734b728" memberName="btnPlay" virtualName=""
              explicitFocusOrder="0" pos="152 407 72 40" tooltip="play newly generated musical piece&#10;"
              buttonText="Play" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="cmbStyle" id="ead1f599e342b918" memberName="cmbStyle" virtualName=""
            explicitFocusOrder="0" pos="32 72 152 24" editable="0" layout="33"
            items="Mozart&#10;Vivaldi&#10;SteveWonder" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="cmbSongs" id="cbb73daa0a3e2e80" memberName="cmbSongs" virtualName=""
            explicitFocusOrder="0" pos="33 144 295 24" editable="0" layout="33"
            items="song1&#10;song2&#10;song3" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="lblStyle" id="21310caaae512075" memberName="lblStyle" virtualName=""
         explicitFocusOrder="0" pos="27 40 150 24" textCol="ff525351"
         edTextCol="ff000000" edBkgCol="0" labelText="Choose musician:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="lblSong" id="b0718847496d6e82" memberName="lblSong" virtualName=""
         explicitFocusOrder="0" pos="28 112 150 24" tooltip="choose the song from predefined list to be used as a seed for music generation"
         textCol="ff525351" edTextCol="ff000000" edBkgCol="0" labelText="Choose song:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <GROUPCOMPONENT name="groupParams" id="25e632ae60edfa98" memberName="groupParams"
                  virtualName="" explicitFocusOrder="0" pos="32 192 416 192" title="Parameters"/>
  <SLIDER name="sliderLen" id="f75ace0c5c8a2043" memberName="sliderLen"
          virtualName="" explicitFocusOrder="0" pos="160 334 265 24" min="10.0"
          max="500.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="lblSongLen" id="d1687451ceaceb1e" memberName="lblSongLen"
         virtualName="" explicitFocusOrder="0" pos="158 294 266 32" tooltip="Choose the number of notes/chords to generate"
         edTextCol="ff000000" edBkgCol="0" labelText="How many notes to generate:&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <COMBOBOX name="cmbDepth" id="4757e736e764c089" memberName="cmbDepth" virtualName=""
            explicitFocusOrder="0" pos="48 334 96 24" tooltip="choose the depth of the pretrained LSTM model"
            editable="0" layout="33" items="15&#10;50&#10;100" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="lblDepth" id="198a5c81dd377098" memberName="lblDepth" virtualName=""
         explicitFocusOrder="0" pos="48 294 96 32" edTextCol="ff000000"
         edBkgCol="0" labelText="LSTM Depth:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="btnSave" id="7dec4a87eb71e24e" memberName="btnSave" virtualName=""
              explicitFocusOrder="0" pos="240 408 80 40" buttonText="Save..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnPlayStyleSong" id="25509d4981a10cb5" memberName="btnPlayStyleSong"
              virtualName="" explicitFocusOrder="0" pos="342 136 104 40" tooltip="play newly generated musical piece&#10;"
              buttonText="Play" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="lblStyleDescription" id="992cee9b5172ca6d" memberName="lblStyleDescription"
         virtualName="" explicitFocusOrder="0" pos="208 60 240 48" edTextCol="ff000000"
         edBkgCol="0" labelText="Which composer's style do you want to use for your new song? Choose composer and initial musical piece&#10;&#10; "
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="5425be36f930a444" memberName="lblModelsFolder"
         virtualName="" explicitFocusOrder="0" pos="46 216 146 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Path to models:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="a33a9e372420312d" memberName="txtPath"
              virtualName="" explicitFocusOrder="0" pos="48 248 320 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="daa8552af4228fef" memberName="btnChoosePath"
              virtualName="" explicitFocusOrder="0" pos="380 248 48 25" buttonText="..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnOpenFolder" id="8ae658c505c236ab" memberName="btnOpenFolder"
              virtualName="" explicitFocusOrder="0" pos="336 408 112 40" buttonText="Open folder..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

