/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "ChordDetector.h"
#include <memory>
#include <map>

//==============================================================================
/**
*/
class LstmMusicProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    LstmMusicProcessor();
    ~LstmMusicProcessor() override;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const juce::String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //generate new melody via selected lstm model
    bool generateMelody(const std::string& style, const std::string& song, int lstmInputLength, int notesCount);
    
    //save last generated midi melody into .mid file
    void saveMidi(const std::string& path);
    
    //stop playing midi
    void stopPlayingMidi();
    
    //play the selected musical piece
    void playInitSong(const std::string& style, const std::string& song);
    
    //play last generated midi melody
    void playGeneratedSong();
    
    //setting current style when user choose a composer in comboBox
    void setCurrentStyle(const std::string& style);
    
    //init models from the given folder
    //to be correctly read, the structure must be as in github 'models/' folder
    void initModels(const std::string& path);
    
    //get methods: after the folder with models is loaded, you can get available data
    std::vector<int> getInputLengths();
    std::vector<std::string> getMusicalStyles() const;
    std::vector<std::string> getTestSongs();
    
    //callbacks for interaction with PluginEditor
    using voidCallback = std::function<void()>;
    void setSongIsFinishedCallback(voidCallback);
    void setGenerationCompletedCallback(voidCallback);
    
    using progressCallback = std::function<void(int, int)>;
    void setProgressCallback(progressCallback);
    
    //flag which indicates that the models have been loaded
    //needed just for showing/not showing the hint
    bool modelsAreLoaded{false};
private:
    //main structure which contains all the paths to models and midi songs
    struct MusicData
    {
        std::map<int, std::string> availableLstmModels;
        std::map<std::string, std::string> availableSongs;
    };
    std::map<std::string, MusicData> musicDict;
    
    //read available midi songs from the given path
    std::map<std::string, std::string> getSongs(const std::string& path);
    
    //current style selected by user
    std::string currentStyle;
    
    /** Vocabulary section **/
    
    //read vocabulary via nlohmann::json lib from the given path
    bool initVocabulary(const std::string& path);
    
    //vocabularies which are used by LSTM model, 1 vocabulary for each composer(style)
    std::map<std::string, int> vocabulary;
    std::map<int, std::string> vocabReverse;
    int vocabSize{0};
    
    //helpers methods which convert a vector of vocabulary indexes(actually, keys) into
    //a vector of notes/chords string representation (actually, values) and vice versa
    //vocabulary and vocabReverse are used for this purpose
    std::vector<int> notesToIndices(const std::vector<std::string>&);
    std::vector<std::string> indicesToNotes(const std::vector<int>&);
    
    /** Callbacks section **/
    
    voidCallback songIsFinishedCallback;
    progressCallback noteGeneratedCallback;
    voidCallback generationCompletedCallback;
    
    /** Playback section **/
    
    //vector of midiMessages that will be played if user clicks 'Play' button
    //in Style section
    std::vector<juce::MidiMessage> initMidiMessagesToPlay;
    
    //vector of newly generated midiMessages that will be played if user
    //clicks 'Play' button in Generation section.
    std::vector<juce::MidiMessage> generatedMidiMesssagesToPlay;
    
    //this method forms 'generatedMidiMesssagesToPlay' vector of midiMessages with proper
    //timestamps to be played when user clicks 'Play' in Generation section
    void buildgeneratedMidiMesssagesToPlay();
    
    //reference to a proper vector with midiMessages
    std::vector<juce::MidiMessage>& midiToPlay = initMidiMessagesToPlay;
    
    //if a user clicks 'Stop' while playing midi, we need to make sure that
    //all noteOffs are sent (otherwise, the notes without noteOff event will sound
    //for a long time)
    bool needToSendNoteOffs{false};
    
    //vector of output indixes of the generated notes according to the dictionary
    std::vector<int> outputNotes;
    
    //the opposite, vector of string representation of the generated notes/chords
    std::vector<std::string> outputNotesStr;
    
    //flag to indicate in processBlock to play midiMessages
    bool canPlayMidi{false};
    
    //index of the current note which is being played
    int currentNote{0};
    
    //keeps the number of elapsed sapmles to process correctly
    //the timing of midiMessages in processBlock
    unsigned long elapsedSamples{0};
    
    //sampling rate
    double sampleRate{0.0f};
    
    //5msec for 1 tick is used, it's needed to play the generated sequence of notes
    const double msecPerTick{0.005f};
    
    //it's needed when writing generated midi notes into a file
    const int ticksPerQuarterNote{96};
    
    //length of the notes in ticks, i wanted shorter than a quarter notes duration
    const int noteLengthInTicks{56};
    
    /** Internal helper methods  **/
    
    //read midi events from the file
    bool readMidi(const std::string& path);
    
    //vector of notes/chords of the selected song
    std::vector<std::string> initMidiNotes;
    
    //extract pattern of the given length from initMidiNotes starting with random index
    //so, each time user clicks 'Generate', the init pattern assumed to be different
    std::vector<std::string> extractPattern(int length);
    
    //normalize indexes of the notes/chords according to the vocabulary size
    std::vector<float> normalizeVector(const std::vector<int>& data, int vocabSize);
    
    //this method generates notes in different thread to avoid freezing the gui thread
    void runGeneration(const std::vector<int>& pattern, int notesCount, const std::string& modelPath);
    
    //this method writes the generated midi notes into a .mid file
    void writeMidiFile(const std::string& midiPath);
    
    //internal method when 'Play' button is clicked
    void playMidi();
    
    //helper class which helps to parse the midi sequence into sequence of chords
    ChordDetector chordDetect;
    
    //return note/chord if it's ready, or empty string if notes are still being read from the
    //sequence
    std::string learnNotes();

    //converts vector of integers to a string representation: <1,2,3> => "1.2.3"
    static std::string notesArrayToString(const std::vector<int>& notesVec);

    //converts string representation into a vector of integers: "1.2.3" => <1,2,3>
    static std::vector<int> stringToNotesArray(const std::string& notesStr);


      //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LstmMusicProcessor)
};
