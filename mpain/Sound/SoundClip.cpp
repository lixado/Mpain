//
// Created by Andreas Høiberg Eike on 2019-10-24.
//

#include <iostream>
#include <SFML/Audio/Sound.hpp>
#include "SoundClip.h"

//Creates a SoundClip. Note that this type should only be used for short sound effects, not long music tracks. For longer tracks, use sf::Music.
//Function returns true if it succeeded and false if it failed.
bool SoundClip::create(const std::string& filepath) {

    //Checks for file access errors.
    if(!buffer.loadFromFile(filepath)) {
        std::cout << "Error loading sound from file." << std::endl;
        return false;
    }
    //Sets the SoundClip to the supplied soundfile
    sound.setBuffer(buffer);

    return true;

}

//Plays the soundclip. can also loop the clip(set looping to true)
void SoundClip::play() {

    //Checks if the buffer exists, for error checking. Check where you play the sound that the sound name is correct.
    if (buffer.getDuration() != sf::Time::Zero) {

        if(sound.getStatus() != sf::SoundSource::Playing) {
            sound.setBuffer(buffer);
            sound.play();
        }
    }

    else
        std::cout << "No such sound exists. Check the sound name." << std::endl;
}

void SoundClip::stop() {
    sound.stop();
}

void SoundClip::setVolume(const float& volume) {
    sound.setVolume(volume);
}