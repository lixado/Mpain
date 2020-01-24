//
// Created by Andreas Høiberg Eike on 2019-10-25.
//

#ifndef MPAIN_SOUNDCLIP_H
#define MPAIN_SOUNDCLIP_H

#include <string>
#include <SFML/Audio.hpp>

//Class for SoundClips, which is used to play short sound effects in the game.
//The file specified must be of the waveform (.wav) format, FLAC format or ORBIS/OGG format.
class SoundClip {

public:

    bool create(const std::string& filepath);
    void play();
    void setVolume(const float& volume);
    void stop();


protected:
    sf::Sound sound;
    sf::SoundBuffer buffer;
};


#endif //MPAIN_SOUNDCLIP_H
