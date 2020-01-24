//
// Created by jakimi on 15.11.2019.
//

#ifndef MPAIN_PLAYERANIMATION_H
#define MPAIN_PLAYERANIMATION_H

#include "Player.h"
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "../machine.h"
#include <SFML/Graphics.hpp>
#include "../Animation/AnimatedSprite.h"



class PlayerAnimation
{
public:
    PlayerAnimation(Player &character);
    void Draw(sf::RenderWindow& window, Player &character);

    // Boss
    enum class DirectionID {RIGHT, LEFT, NONE, UP , DOWN};
    void BossDraw(sf::RenderWindow* window, sf::Vector2f pos, bool draw, int attack);

    bool attack3 = false;
    sf::Clock attack3clock;

    bool attack1 = false;
    sf::Clock attack1clock;

    bool attack4 = false;
    sf::Clock attack4clock;


    float aniSpeed = 1;

    sf::Clock frameTimeboss;
protected:

    sf::Texture texture;
    Animation walkingLeft;
    Animation walkingRight;
    Animation duckingLeft;
    Animation duckingRight;
    Animation jumpRight;
    Animation jumpLeft;
    Animation *currentAnimation;
    AnimatedSprite animeSprite;
    sf::Clock frameClock;
    enum class sidesID {RIGHT, LEFT, UP};
    sidesID sides;

    // Boss animation
    sf::Texture bossRightTexture;
    sf::Texture bossLeftTexture;
    sf::Texture bossLaserAttackTexture;
    sf::Texture weak_texture;
    Animation flyingRight;
    Animation flyingLeft;
    Animation shootingLaser;
    Animation *bossCurrentAnimation;
    AnimatedSprite bossAnimeSprite;
    Animation weak;
};

#endif //MPAIN_PLAYERANIMATION_H
