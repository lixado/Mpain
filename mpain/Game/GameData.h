//
// Created by fotba on 15.11.2019.
//

#ifndef MPAIN_GAMEDATA_H
#define MPAIN_GAMEDATA_H

#include <map>
#include "../Sound/SoundClip.h"
#include "../Sound/Music.h"
#include "../Configurations/Configuration.h"


class GameData {

    public:

    GameData();
    void setVolume(const float& volume);
    float getVolume();

    void playMusic(const std::string& filename, const bool& looping);
    void stopMusic(const std::string& filename);

    void playSound(const std::string& name);
    void stopSound(const std::string& name);

    //load sounds
    SoundClip clip;
    sf::Music music;
    std::map<std::string, SoundClip> sounds;
    std::vector<std::unique_ptr<Music>> songs;

    Configuration config;


};


#endif //MPAIN_GAMEDATA_H
