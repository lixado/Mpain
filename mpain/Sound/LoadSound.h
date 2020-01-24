//
// Created by Andreas Høiberg Eike on 2019-11-08.
//

#ifndef MPAIN_LOADSOUND_H
#define MPAIN_LOADSOUND_H

#include <vector>
#include <map>
#include "SoundClip.h"
#include "Music.h"
#include "../Configurations/Configuration.h"

std::map<std::string, SoundClip> loadSoundClips(SoundClip& clip, const Configuration& config);

std::vector<std::unique_ptr<Music>> loadMusic(const Configuration& config);

void playMusic(const std::vector<std::unique_ptr<Music>>& music, const std::string& filename, const bool& looping);



#endif //MPAIN_LOADSOUND_H
