# LSTM classical music generator plugin
 Project within the course "Selected Topics in Music and Acoustic Engineering", prof. Yee-King Matthew John  
 Politecnico di Milano, 2023-2024  
 Built and tested on macOs

# Project description
This project aims to experiment with LSTM models with different context lengths to generate music in the style of 3 classical composers: Mozart, Bach and Chopin.

# How to build from the source
1. The project has the following dependencies, so before starting, you have to install them:  
1.1 frugally-deep https://github.com/Dobiasd/frugally-deep/blob/master/INSTALL.md  
1.2 nlohmann/json https://github.com/nlohmann/json  
1.3 Juce https://juce.com/  
To build your project, cmake needs to know where JUCE is on your system.  
Probably, you need to update the add_subdirectory command in CMakeLists to point at your JUCE directory. On my system, I have it here:  
add_subdirectory(/Applications/JUCE ./JUCE)   
The first ‘/Applications/JUCE’ indicates where JUCE can be found. The second part ‘./JUCE’ defines where JUCE will be copied to in the build folder.  

2. From the root project folder, run "cmake -G "Xcode" -B build". The .xcodeproj will be in the 'build' folder.

# How to train the model
In the 'python' folder you can find an example how to train the LSTM model. Also, there are datasets that I used for this project. If there is a wish to train
a model with a different dataset(to experiment with another musical style), it can be easily done by correcting the paths in the notebook and uploading proper midi files within the dataset.  
# How to use
1. If you build the project from the source code, it will install the plugin into the default location
(/Users/<username>/Library/Audio/Plug-Ins/VST3 or /Users/<username>/Library/Audio/Plug-Ins/Components)

2. If no, you have to download the plugin from bin/ folder and place it in the plugin location on your system  
3. Once done, you should be able to see lstm_classical_music_generator in the plugin's list in your DAW
4. Also, you need to download /models folder (it contains pre-trained models in a suitable for frugally-deep format, as well as the vocabularies
   for each musical style and initial midi songs) and place it somewhere on your disc
6. You need to load the models to the plugin by choosing the folder with the models. The button is in the 'parameters' block  
7. Finally, you can select the composer, initial song, length of the context and the number of notes to generate. Then, click 'Generate' and wait until a new melody is generated

The demo video and brief presentation of the project is also available: [LSTM Classical Music Generator](https://www.youtube.com/watch?v=C4yEzb9eYeY)

![alt text](https://github.com/Oliffka/lstm_classical_music_generator/blob/main/image/plugin.png?raw=true)



