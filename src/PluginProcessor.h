/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

// #include <JuceHeader.h>
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
    /** add some midi to be played at the sent sample offset*/
    void addMidi(juce::MidiMessage msg, int sampleOffset);
    /**
     * @brief reset the markov model
     * 
     */
    void resetModel();
    // check if the markov model is learning from incoming midi
    bool isLearning();
    // stop the markov model from learning from incoming MIDI
    void stopLearning();
    // start the markov model learning from incoming MIDI
    void startLearning();
    // generate new notes sequence using chosen lstm model
    bool generateMelody(const std::string& style, const std::string& song, int lstmDepth, int notesCount);
    
    void saveMidi(const std::string&);
    
    void playMidi();
    
    void stopPlayingMidi();
    
    void playInitSong(const std::string& style, const std::string& song);
    
    void playGeneratedSong();
    
    bool readMidi(const std::string& path, bool considerVocab = true);
    
    void setCurrentStyle(const std::string& style);
    
    void initModels(const std::string& path);
    
    std::vector<int> getLstmDepths();
    std::vector<std::string> getMusicalStyles();
    std::vector<std::string> getTestSongs();
    
    using voidCallback = std::function<void()>;
    void setSongIsFinishedCallback(voidCallback);
    void setGenerationCompletedCallback(voidCallback);
    
    using progressCallback = std::function<void(int, int)>;
    void setProgressCallback(progressCallback);

private:
    struct MusicData
    {
        std::map<int, std::string> availableLstmModels;
        std::map<std::string, std::string> availableSongs;
    };
    
    std::map<std::string, std::string> getSongs(const std::string& path);
    
    voidCallback songIsFinishedCallback;
    progressCallback noteGeneratedCallback;
    voidCallback generationCompletedCallback;
    
    std::map<std::string, MusicData> musicDict;
    std::string currentStyle = "mozart";
    
    std::map<std::string, int> vocabulary;
    std::map<int, std::string> vocabReverse;
    
    bool canPlayMidi{false};
    
    bool learningMode;
    int vocabSize{0};
    std::vector<std::string> extractPattern(int maxLength);
    std::vector<float> normalizeVector(const std::vector<int>& data, int vocabSize);
    
    void runGeneration(const std::vector<int>& pattern, int notesCount, const std::string& modelPath);
    void writeMidiFile(const std::string& midiPath);
    juce::MidiMessageSequence buildMidiSequence();
    bool initVocabulary(const std::string& path);
    
    std::vector<int> notesToIndices(const std::vector<std::string>&);
    std::vector<std::string> indicesToNotes(const std::vector<int>&);
    
    /** stores messages added from the addMidi function*/
    juce::MidiBuffer midiToProcess;
    juce::MidiBuffer midiToPlay;
    std::vector<std::string> initMidiNotes;
    
    std::vector<int> outputNotes;
    std::vector<std::string> outputNotesStr;
    std::vector<std::string> notesToPlay;
    
    
    int noteDuration; //note duration in samples
    int currentNote = 0;
    std::string lastNotes{""};
    int time = 0;
    unsigned long elapsedSamples;
    
    ChordDetector chordDetect;

    
    // add the notes stored in the chord detector to the model
    std::string learnNotes();

        
    /**
     * @brief converts a vector of midi note values to a single string representation
     * 
     * @param notesVec 
     * @return std::string 
     */
    static std::string notesArrayToString(const std::vector<int>& notesVec);
    /**
     * @brief converts a string representation of notes from notesArrayToString back to an int vector of midi note values
     *
     * @param notesStr 
     * @return std::vector<int> 
     */
    static std::vector<int> stringToNotesArray(const std::string& notesStr);


      //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LstmMusicProcessor)
};
