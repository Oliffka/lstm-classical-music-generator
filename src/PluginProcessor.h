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

    //generate new melody using selected lstm model
    bool generateMelody(const std::string& style, const std::string& song, int lstmDepth, int notesCount);
    
    //save last generated midi
    void saveMidi(const std::string&);
    
    //internal method
    void playMidi();
    
    //stop playing midi
    void stopPlayingMidi();
    
    //play the selected musical piece
    void playInitSong(const std::string& style, const std::string& song);
    
    //play last generated midi
    void playGeneratedSong();
    
    //read midi events from the file
    bool readMidi(const std::string& path);
    
    //occur when user choose a composer
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
    
    //flag which indicates the models have been loaded
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
    
    //vocabularies which are used by the model, 1 vocabulary for each composer(style)
    std::map<std::string, int> vocabulary;
    std::map<int, std::string> vocabReverse;
    int vocabSize{0};
    
    //read available midi songs from the given path
    std::map<std::string, std::string> getSongs(const std::string& path);
    
    //callbacks
    voidCallback songIsFinishedCallback;
    progressCallback noteGeneratedCallback;
    voidCallback generationCompletedCallback;
    
    //current style selected by user
    std::string currentStyle;
    
    std::vector<std::string> initMidiNotes;
    std::vector<std::string> extractPattern(int maxLength);
    std::vector<float> normalizeVector(const std::vector<int>& data, int vocabSize);
    
    void runGeneration(const std::vector<int>& pattern, int notesCount, const std::string& modelPath);
    void writeMidiFile(const std::string& midiPath);
    bool initVocabulary(const std::string& path);
    void buildGeneratedMidiToPlay();
    
    std::vector<int> notesToIndices(const std::vector<std::string>&);
    std::vector<std::string> indicesToNotes(const std::vector<int>&);
    
    //ugly way to indicate that there is no last NoteOn event
    bool needToSendNoteOffs{false};


    /** Playback section **/
    
    //sequence of midiMessages which will be played if user clicks 'Play' button
    //in Style section
    std::vector<juce::MidiMessage> initMidiToPlay;
    
    //sequence of newly generated midiMessages which will be played if user
    //clicks 'Play' button in Generation section.
    std::vector<juce::MidiMessage> generatedMidiToPlay;
    
    //reference to the proper sequence
    std::vector<juce::MidiMessage>& midiToPlay = initMidiToPlay;
    
    std::vector<int> outputNotes;
    std::vector<std::string> outputNotesStr;
    
    bool canPlayMidi{false};
    int currentNote = 0;
    unsigned long elapsedSamples = 0;
    double sampleRate = 0.0f;
    
    const double msecPerTick = 0.005;
    const int ticksPerQuarterNote = 96;
    const int noteLengthInTicks = 56;
    
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
