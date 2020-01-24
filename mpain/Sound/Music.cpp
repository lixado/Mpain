//
// Created by Andreas Høiberg Eike on 2019-10-24.
//

#include <iostream>

#include "Music.h"

//Creates a music clip. Returns true if success and false if fail.
bool Music::create(const std::string& filepath, const std::string& name) {

    //Checks for file opening errors
    if(!music.openFromFile(filepath)) {
        std::cout << "Error loading sound from file." << std::endl;
        return false;
    }

    this->filename = name;

    return true;
}

//Plays the music clip. Set looping to true if you want to loop the track until it's paused or stopped.
void Music::play(bool looping) {
    music.setLoop(looping);

    if(music.getStatus()!=sf::Music::Playing)
        music.play();
}

void Music::stop() {
    music.stop();
}

void Music::setVolume(const float& volume) {
    //std::cout << volume << std::endl;

    music.setVolume(volume);
}

float Music::getVolume() {
    return music.getVolume();
}
