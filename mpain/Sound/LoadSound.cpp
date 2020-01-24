//
// Created by Andreas Høiberg Eike on 2019-11-08.
//

#include <map>
#include "SoundClip.h"
#include "Music.h"
#include "../Configurations/Configuration.h"

//Load sounds and store them in a map. To access the sound and play it: soundClips["nameOfSound"].play()
//Should add all the sounds manually, using the template shown below for loading jump sound effect.
std::map<std::string, SoundClip> loadSoundClips(SoundClip& clip, const Configuration& config) {

    std::map<std::string, SoundClip> soundClips;

    //Creates a SoundClip and inserts it into the soundsClips map.
    clip.create(config.jumpSoundEffect);
    soundClips[config.jumpSoundEffect] = clip;

    clip.create(config.deathSoundEffect);
    soundClips[config.deathSoundEffect] = clip;

    clip.create(config.boss_roar);
    soundClips[config.boss_roar] = clip;

    clip.create(config.blind);
    soundClips[config.blind] = clip;

    return soundClips;
}

std::vector<std::unique_ptr<Music>> loadMusic(const Configuration& config) {
    std::vector<std::unique_ptr<Music>> musicClips;

    auto music = std::make_unique<Music>();

    music->create(config.map1Music, config.map1Music);
    musicClips.push_back(std::move(music));

    music = std::make_unique<Music>();

    music->create(config.menuMusic, config.menuMusic);
    musicClips.push_back(std::move(music));

    return musicClips;
}