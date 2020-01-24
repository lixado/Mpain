//
// Created by Andreas on 27.10.2019.
//

#ifndef MPAIN_CONFIGURATION_H
#define MPAIN_CONFIGURATION_H

#include <iostream>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>


//Settings for sfml window
class Configuration {

    public:
        const int screenWidth = 960;
        const int screenHeight = 960;
        const std::string name = "Super Mariochisme";

        //Maps
        const std::string map_1 = "Maps/Map 1.json";
        const sf::Vector2f map_1_start = {32,600};

        const std::string map_2 = "Maps/Map 2.json";
        const sf::Vector2f map_2_start = {32,700};

        const std::string map_3 = "Maps/Map 3.json";
        const sf::Vector2f map_3_start = {32,800};

        const std::string map_4 = "Maps/Map 4.json";
        const sf::Vector2f map_4_start = {32,170};

        const std::string map_5 = "Maps/Map 5.json";
        const sf::Vector2f map_5_start = {896,832};

        const std::string map_6 = "Maps/Map 6.json";
        const sf::Vector2f map_6_start = {864,864};

        const std::string map_7 = "Maps/Map 7.json";
        const sf::Vector2f map_7_start = {10,225};

        const std::string map_8 = "Maps/Map 8.json";
        const sf::Vector2f map_8_start = {32,768};

        const std::string map_9 = "Maps/Map 9.json";
        const sf::Vector2f map_9_start = {32,768};

        const std::vector<std::string> Maps = {map_1, map_2, map_3, map_4, map_5, map_6, map_7, map_8, map_9};
        const std::vector<sf::Vector2f> Map_Start = {map_1_start, map_2_start, map_3_start, map_4_start, map_5_start, map_6_start, map_7_start, map_8_start, map_9_start};

        //variables for the game music
        const std::string menuMusic = "Assets/sound/menu.wav";
        const std::string map1Music = "Assets/sound/map1.wav";

        //Variables for the sound effects
        const std::string jumpSoundEffect = "Assets/sound/jump.wav";
        const std::string deathSoundEffect = "Assets/sound/deathsound.wav";
        const std::string boss_roar = "Assets/sound/Lion-Roar.wav";
        const std::string blind = "Assets/sound/Flashbang.wav";

    //Statesettings background
        const std::string settings_background = "Assets/Images/background+settings.png";

        //so the game cant kill
        bool GodMode = false;
};

#endif //MPAIN_CONFIGURATION_H
