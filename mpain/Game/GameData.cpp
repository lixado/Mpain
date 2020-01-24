//
// Created by fotba on 15.11.2019.
//

#include <iostream>
#include "GameData.h"
#include "../Sound/LoadSound.h"

GameData::GameData(){

    songs = loadMusic(config);
    sounds = loadSoundClips(clip, config);

}

void GameData::setVolume(const float& volume) {

    for(auto& n : songs) {

        n->setVolume(volume);
    }

    for(auto& n : sounds) {
        n.second.setVolume(volume);
    }
}

float GameData::getVolume() {
    return songs[0]->getVolume();
}

//Plays the music. Since we have to store the music clips in a vector, you must search for the given music clip with this function.
void GameData::playMusic(const std::string& filename, const bool& looping) {

    for(const auto& n: songs ) {
        if(n->filename == filename) {
            n->play(looping);
        }
    }
}

void GameData::stopMusic(const std::string& filename) {
    for (const auto& n: songs) {
        if(n->filename == filename) {
            n->stop();
        }
    }
}

void GameData::playSound(const std::string& name) {
    sounds[name].play();
}

void GameData::stopSound(const std::string& name) {
    sounds[name].stop();
}