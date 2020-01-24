//
// Created by Andreas Høiberg Eike on 2019-10-25.
//

#ifndef MPAIN_MUSIC_H
#define MPAIN_MUSIC_H

#include <SFML/Audio/Music.hpp>

//Class for playing music clips, such as longer background music tracks.
//For short sound effects you should use the SoundClip class.
//Files loaded must be of wav, OGG/Orbis og FLAC format.
class Music {

public:
    bool create(const std::string& filepath, const std::string& name);

    std::string filename;
    void play(bool looping);
    void stop();
    void setVolume(const float& volume);
    float getVolume();



protected:
    sf::Music music;

};

#endif //MPAIN_MUSIC_H
