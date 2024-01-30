/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <filesystem>
#include <string>
#include <future>

#include <fstream>
#include <nlohmann/json.hpp>

#include <fdeep/fdeep.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;
  
//==============================================================================
LstmMusicProcessor::LstmMusicProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                    .withInput("Input", juce::AudioChannelSet::mono(), true)
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
, chordDetect{100}, learningMode{false}
{
}

LstmMusicProcessor::~LstmMusicProcessor()
{
}

void LstmMusicProcessor::setSongIsFinishedCallback(voidCallback callback)
{
    songIsFinishedCallback = callback;
}

void LstmMusicProcessor::setProgressCallback(progressCallback callback)
{
    noteGeneratedCallback = callback;
}

void LstmMusicProcessor::setGenerationCompletedCallback(voidCallback callback)
{
    generationCompletedCallback = callback;
}

void LstmMusicProcessor::initModels(const std::string& modelPath)
{
    if (!fs::exists(modelPath))
        return;

    musicDict.clear();
    
    for (const auto & entry : fs::directory_iterator(modelPath))
    {
        if (fs::is_directory(entry))
        {
            fs::path entryPath(entry);
            
            std::string styleFolderName = entryPath.filename().string();
            std::cout << styleFolderName << std::endl;
            
            MusicData currentData;
            for (const auto & curPath : fs::directory_iterator(entry))
            {
                if (fs::is_directory(curPath))
                {
                    fs::path curFolder(curPath);
                    
                    std::string folderName = curFolder.filename().string();
                    if (folderName.compare("midi") == 0)
                    {
                        currentData.availableSongs = getSongs(curPath.path());
                    }
                    else
                    {
                        int lstmDepth = -1;
                        try
                        {
                           lstmDepth = std::stoi(folderName);
                        }
                        catch (...)
                        {
                           std::cout << "error: can't parse folder name: " << folderName;
                            continue;
                        }
                        if (lstmDepth > 0)
                        {
                            auto curModelPath = curFolder;
                            curModelPath /= (styleFolderName + "_" + folderName + ".json");
                            
                            //auto curModelPath = curPath / (styleFolderName + "_" + folderName + ".json");
                            currentData.availableLstmModels.insert({lstmDepth, curModelPath.string()});
                        }
                    }
                }
            }
            musicDict.insert({styleFolderName, currentData});
        }
    }
}

std::map<std::string, std::string> LstmMusicProcessor::getSongs(const std::string& path)
{
    std::map<std::string, std::string> songs;
    for (const auto & curPath : fs::directory_iterator(path))
    {
        if (fs::is_regular_file(curPath))
        {
            fs::path curSong(curPath);
            
            const auto ext = curSong.extension();
            if (ext == ".mid" || ext == ".midi")
            {
                std::string songName = curSong.filename().string();
                songs.insert({songName, curPath.path()});
                
            }
        }
    }
    return songs;
}

void LstmMusicProcessor::setCurrentStyle(const std::string& style)
{
    currentStyle = style;
}

std::vector<int> LstmMusicProcessor::getLstmDepths()
{
    std::vector<int> depths;
    
    const auto& data = musicDict[currentStyle];
    for(auto const& model: data.availableLstmModels)
        depths.push_back(model.first);
    
    return depths;
}

std::vector<std::string> LstmMusicProcessor::getMusicalStyles()
{
    std::vector<std::string> styles;
    
    for(auto const& style: musicDict)
        styles.push_back(style.first);
    
    return styles;
}

std::vector<std::string> LstmMusicProcessor::getTestSongs()
{
    std::vector<std::string> songs;
    const auto& data = musicDict[currentStyle];
    for(auto const& song: data.availableSongs)
        songs.push_back(song.first);
    
    return songs;
}

//==============================================================================
const juce::String LstmMusicProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LstmMusicProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LstmMusicProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LstmMusicProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LstmMusicProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LstmMusicProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LstmMusicProcessor::getCurrentProgram()
{
    return 0;
}

void LstmMusicProcessor::setCurrentProgram (int index)
{
}

const juce::String LstmMusicProcessor::getProgramName (int index)
{
    return {};
}

void LstmMusicProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LstmMusicProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // here we know the sample rate so can set the
    // maxinterval properly on the chord detector
    // 0.5ms seems to be about right
    double maxIntervalInSamples = sampleRate * 0.001; // 1ms
    //std::cout << "Max interval in samples " << maxIntervalInSamples << " or " << (maxIntervalInSamples * 96000 * 1000) << "ms";
    chordDetect = ChordDetector((unsigned long) maxIntervalInSamples);
    noteDuration = static_cast<int> (std::ceil (sampleRate * 0.25f));
}

void LstmMusicProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LstmMusicProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LstmMusicProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    midiMessages.clear();
    if (!canPlayMidi)
    {
        return;
    }
    
    auto numSamples = buffer.getNumSamples();
    
    if ((time + numSamples) >= noteDuration)
    {
        auto offset = juce::jmax (0, juce::jmin ((int) (noteDuration - time), numSamples - 1));

        if (lastNotes != "")
        {
            auto notesArray = stringToNotesArray(lastNotes);
            for (auto& note: notesArray)
            {
                midiMessages.addEvent(juce::MidiMessage::noteOff (1, note), offset);
                std::cout<<"noteOff event: " <<note<< "; offset: "<<offset<<std::endl;

            }
            lastNotes = "";
        }

        if (currentNote < notesToPlay.size())
        {
            lastNotes = notesToPlay[currentNote];
            auto notesArray = stringToNotesArray(lastNotes);
            if (notesArray.size() > 1)
                DBG("Here is the chord!!");
            
            for (auto& note: notesArray)
            {
                midiMessages.addEvent(juce::MidiMessage::noteOn(1, note, (juce::uint8) 127), offset);
                std::cout<<"noteOn event: " <<note<< "; offset: "<<offset<<std::endl;
            }
            currentNote += 1;
        }
        else if (songIsFinishedCallback)
        {
            if (lastNotes != "")
            {
                auto notesArray = stringToNotesArray(lastNotes);
                for (auto& note: notesArray)
                {
                    midiMessages.addEvent(juce::MidiMessage::noteOff (1, note), offset);
                }
                lastNotes = "";
            }
            
            songIsFinishedCallback();
            stopPlayingMidi();
        }
    }

    time = (time + numSamples) % noteDuration;
}

std::vector<std::string> LstmMusicProcessor::extractPattern(int maxLength)
{
    if(initMidiNotes.size() < maxLength)
    {
        assert("input length of the current song is less than required");
        return std::vector<std::string>{};
    }
    
    int startIndex = juce::Random::getSystemRandom().nextInt(initMidiNotes.size() - maxLength + 1);
    
    return std::vector<std::string>(initMidiNotes.begin() + startIndex,initMidiNotes.begin() + startIndex + maxLength);
}

std::vector<float> LstmMusicProcessor::normalizeVector(const std::vector<int>& data, int vocabSize)
{
    std::vector<float> normalizedData;
    
    for (auto& x : data)
    {
        normalizedData.push_back(float(x) / vocabSize);
    }
    return normalizedData;
}

std::vector<int> LstmMusicProcessor::notesToIndices(const std::vector<std::string>& notes)
{
    std::vector<int> indices;
    
    for (auto& x : notes)
    {
        indices.push_back(vocabulary[x]);
    }
    return indices;
}

std::vector<std::string> LstmMusicProcessor::indicesToNotes(const std::vector<int>& indices)
{
    std::vector<std::string> notes;
    
    for (auto& x : indices)
    {
        notes.push_back(vocabReverse[x]);
    }
    return notes;
}

bool LstmMusicProcessor::generateMelody(const std::string& style, const std::string& song, int lstmDepth, int notesCount)
{
    // Initialize model
    auto curMusicData = musicDict[style];
    const auto modelPathStr = curMusicData.availableLstmModels[lstmDepth];
    const auto songsPathStr = curMusicData.availableSongs[song];
    
    fs::path modelPath{modelPathStr};
    
    fs::path vocabPath = modelPath.parent_path().parent_path();
    vocabPath /= (style + "_vocab.json");
    
    if (!fs::exists(modelPath))
    {
        assert("The model path doesn't exist!");
        return false;
    }
    
    if (!fs::exists(vocabPath))
    {
        assert("The vocabulary path doesn't exist!");
        return false;
    }
    
    if (!fs::exists(fs::path{songsPathStr}))
    {
        assert("The chosen song path doesn't exist!");
        return false;
    }
    
    if (!initVocabulary(vocabPath.string()))
    {
        DBG("Can't init vocabulary");
        return false;
    }
    
    if (!readMidi(songsPathStr))
    {
        DBG("Can't read midi");
        return false;
    }
    
    const auto patternStr = extractPattern(lstmDepth);
    if (patternStr.size() != lstmDepth)
    {
        DBG("Can't extract notes pattern of the required length");
        return false;
    }
    
    auto pattern = notesToIndices(patternStr);

    std::thread progressThread(&LstmMusicProcessor::runGeneration, this, pattern, notesCount, modelPath);
    progressThread.detach();
    
    return true;
}

void LstmMusicProcessor::runGeneration(const std::vector<int>& pattern, int notesCount, const std::string& modelPath)
{
    const auto model = fdeep::load_model(modelPath);
    auto curPattern = pattern;
    canPlayMidi = false;
    outputNotes.clear();
    
    for (auto noteNumber = 0; noteNumber< notesCount; noteNumber++ )
    {
        auto model_input = normalizeVector(curPattern, vocabSize);
        
        const auto best_prediction = model.predict_class_with_confidence ({fdeep::tensor(fdeep::tensor_shape(static_cast<std::size_t>(model_input.size()), static_cast<std::size_t>(1)), model_input)}).first;

        outputNotes.push_back(best_prediction);
        
        std::cout << "output index: " << best_prediction << "; note: " << vocabReverse[best_prediction]<<std::endl;
        
        curPattern.push_back(best_prediction);
        curPattern.erase(curPattern.begin());
        
        if (noteGeneratedCallback)
        {
            noteGeneratedCallback(noteNumber+1, notesCount);
        }
    }
    
    outputNotesStr = indicesToNotes(outputNotes);
    if (generationCompletedCallback)
    {
        generationCompletedCallback();
    }
}

void LstmMusicProcessor::saveMidi(const std::string& midiPath)
{
    writeMidiFile(midiPath);
}

void LstmMusicProcessor::playGeneratedSong()
{
    notesToPlay = outputNotesStr;
    playMidi();
}

void LstmMusicProcessor::playInitSong(const std::string& style, const std::string& song)
{
    auto curMusicData = musicDict[style];
    const auto songsPathStr = curMusicData.availableSongs[song];
    
    if (!fs::exists(fs::path{songsPathStr}))
    {
        assert("The chosen song path doesn't exist!");
        return;
    }
    
    readMidi(songsPathStr, false);
    notesToPlay = initMidiNotes;
    playMidi();
}

void LstmMusicProcessor::playMidi()
{
    canPlayMidi = true;
}

void LstmMusicProcessor::stopPlayingMidi()
{
    canPlayMidi = false;
    currentNote = 0;
    lastNotes = "";
    time = 0;
}

bool LstmMusicProcessor::initVocabulary(const std::string& vocabPath)
{
    std::ifstream f(vocabPath);
    json data = json::parse(f);

    for (auto& [key, value] : data.items()) 
    {
        std::cout << key << " : " << value << "\n";
        this->vocabulary[key] = value;
        this->vocabReverse[value] = key;
    }
    
    this->vocabSize = vocabulary.size();
    return this->vocabSize > 0;
}

juce::MidiMessageSequence LstmMusicProcessor::buildMidiSequence()
{
    // Add a MidiMessageSequence to store MIDI events
    juce::MidiMessageSequence midiSequence;

    float interval = 50; //msec
    float start = 0;
    
    auto notesStr = indicesToNotes(outputNotes);
    for (auto noteStr: notesStr)
    {
        auto notes = stringToNotesArray(noteStr);
        
        if (notes.size() > 1)
            DBG("Here is the chord!!");
        
        for (auto& note: notes)
        {
            // Create a MIDI message for note-on event
            juce::MidiMessage noteOnMessage = juce::MidiMessage::noteOn(1, note, 1.0f);
            
            // Add note-on message to the sequence at time 0
            midiSequence.addEvent(noteOnMessage, start);
            midiToPlay.addEvent(noteOnMessage, start);
            // Create a MIDI message for note-off event after 1 second (assuming 1 second duration)
            juce::MidiMessage noteOffMessage = juce::MidiMessage::noteOff(1, note);
            
            // Add note-off message to the sequence at time 50ms
            midiSequence.addEvent(noteOffMessage, start + interval);
            midiToPlay.addEvent(noteOffMessage, start + interval);
        }
        start += interval;
    }
    return midiSequence;
}

void LstmMusicProcessor::writeMidiFile(const std::string& midiPath)
{
    juce::MidiFile midiFile;

    // Add a MidiMessageSequence to store MIDI events
    juce::MidiMessageSequence midiSequence;

    float interval = 50; //msec
    float start = 0;
    
    auto notesStr = indicesToNotes(outputNotes);
    for (auto noteStr: notesStr)
    {
        auto notes = stringToNotesArray(noteStr);
        
        if (notes.size() > 1)
            DBG("Here is the chord!!");
        
        for (auto& note: notes)
        {
            // Create a MIDI message for note-on event
            juce::MidiMessage noteOnMessage = juce::MidiMessage::noteOn(1, note, 1.0f);
            
            // Add note-on message to the sequence at time 0
            midiSequence.addEvent(noteOnMessage, start);
            midiToPlay.addEvent(noteOnMessage, start);
            // Create a MIDI message for note-off event after 1 second (assuming 1 second duration)
            juce::MidiMessage noteOffMessage = juce::MidiMessage::noteOff(1, note);
            
            // Add note-off message to the sequence at time 50ms
            midiSequence.addEvent(noteOffMessage, start + interval);
            midiToPlay.addEvent(noteOffMessage, start + interval);
        }
        start += interval;
    }
    
    // Add the MidiMessageSequence to the MidiFile
    midiFile.setTicksPerQuarterNote(96); // Set ticks per quarter note
    midiFile.addTrack(midiSequence);

    // Save the MidiFile to disk
    //std::string outfilePath = "/Users/missolivia/Documents/Juce Projects/STopics/lstm_classical_music_generator/output/out.mid";
    juce::File midiOutputFile(midiPath);
    
    std::unique_ptr<juce::FileOutputStream> outputStream(midiOutputFile.createOutputStream());
    
    bool success = midiFile.writeTo(*outputStream);
    if (!success)
    {
        DBG("something goes wrong");
    }
}

bool LstmMusicProcessor::readMidi(const std::string& path, bool considerVocab)
{
    juce::File file(path);
    
    // Inside the openFile method after loading the MIDI file
    juce::MidiFile midiFile;
    auto midiStream = juce::FileInputStream(file);
    
    if (!midiFile.readFrom(midiStream))
        return false;
    
    initMidiNotes.clear();
    
    const auto track = midiFile.getTrack(0);
    // Process MIDI file
    int totalEvents = track->getNumEvents();
    
    for (auto midiIndex = 0; midiIndex < totalEvents; midiIndex
         ++)
    {
        const auto event = track->getEventPointer(midiIndex);
        
        if (event != nullptr && event->message.isNoteOn())
        {
            auto curMessage = event->message;

            chordDetect.addNote(curMessage.getNoteNumber(), curMessage.getTimeStamp());
            
            auto notes = learnNotes();
            if (!notes.empty())
            {
                if (considerVocab)
                {
                    if (vocabulary.find(notes) != vocabulary.end())
                    {
                        initMidiNotes.push_back(notes);
                    }
                    else
                    {
                        DBG("!!!Chord not found: " + notes);
                    }
                }
                else
                {
                    initMidiNotes.push_back(notes);
                }
            }
        }
    }
    return true;
}

/*
        auto i = startIndex;
        auto midiIndex = startIndex;
        while(i < startIndex + depth && midiIndex < totalEvents)
        {
            const auto event = track->getEventPointer(midiIndex);
            
            if (event != nullptr && event->message.isNoteOn())
            {
                auto curMessage = event->message;
                
                DBG(curMessage.getDescription());
                DBG(curMessage.getTimeStamp());
                
                chordDetect.addNote(curMessage.getNoteNumber(), curMessage.getTimeStamp());
                
                auto notes = learnNotes();
                if (!notes.empty())
                {
                    if (vocabulary.find(notes) != vocabulary.end())
                    {
                        initMidiNotes.push_back(notes);
                        i++;
                        DBG("chord:" + notes);
                    }
                    else
                    {
                        DBG("!!!Chord not found: " + notes);
                    }
                }
            }
            midiIndex++;
        }
    return true;
}*/

//==============================================================================
bool LstmMusicProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LstmMusicProcessor::createEditor()
{
    return new LstmMusicEditor (*this);
}

//==============================================================================
void LstmMusicProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LstmMusicProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LstmMusicProcessor();
}


void LstmMusicProcessor::addMidi(juce::MidiMessage msg, int sampleOffset)
{
  midiToProcess.addEvent(msg, sampleOffset);
}


std::string LstmMusicProcessor::notesArrayToString(const std::vector<int>& notesArray)
{
  std::string state{""};
  for (const auto& note : notesArray)
  {
    state += std::to_string(note) + ".";
  }
    
  if (!state.empty())
  {
      state.pop_back(); //remove last dot
  }
  return state;
}

std::vector<int> LstmMusicProcessor::stringToNotesArray(const std::string& notesStr)
{
    std::vector<int> notes;
    if (notesStr == "0") return notes;

    juce::StringArray tokens;
    tokens.addTokens(notesStr, ".", "\"");

    for (const auto& note : tokens)
    {
        notes.push_back(note.getIntValue());
    }
    return notes;
}

std::string LstmMusicProcessor::learnNotes()
{
    std::string notes;
    if (chordDetect.hasChord())
    {
        notes = notesArrayToString(chordDetect.getChord());
    }
    return notes;
}

