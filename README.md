# lstm_classical_music_generator
 Project within the course "Selected Topics in Music and Acoustic Engineering", 2023-2024  
 Built and tested on MacOs

# How to build from the source
1. The project has the following dependencies, so before starting, you have to install them:  
1.1 frugally-deep https://github.com/Dobiasd/frugally-deep/blob/master/INSTALL.md  
1.2 nlohmann/json https://github.com/nlohmann/json  
1.3 Juce https://juce.com/  
To build your project, cmake needs to know where JUCE is on your system.  
Probably, you need to update the add_subdirectory command to point at your JUCE directory. On my system, I have it here:  
add_subdirectory(/Applications/JUCE ./JUCE)   
The first ‘/Applications/JUCE’ indicates where JUCE can be found. The second part ‘./JUCE’ defines where JUCE will be copied to in the build folder.  

2.From the root project folder, run "cmake -G "Xcode" -B build". The .xcodeproj will be in the 'build' folder.

# How to use
1. If you build the project from the source code, it will install the plugin into the default location
(/Users/<username>/Library/Audio/Plug-Ins/VST3 or /Users/<username>/Library/Audio/Plug-Ins/Components)

2. If no, you have to download plugin from bin/ folder and place it in the plugin location on your system  
3. Once done, you should be able to see lstm_classical_music_generator in the plugin's list in your DAW
4. Also, you need to download /models folder and place it somewhere on your disc
5. You need to load the models to the plugin by choosing the folder with the models. The button is in the 'parameters' block  
6. Finally, you can select the composer, initial song, length of the context and the number of notes to generate. Then, click 'Generate' and wait until a new melody is generated





