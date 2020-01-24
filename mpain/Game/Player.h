//
// Created by pedro on 11/1/19.
//

#ifndef MPAIN_PLAYER_H
#define MPAIN_PLAYER_H


#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <map>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../Configurations/Configuration.h"

class Player {
public:
    Player(sf::RenderWindow* window);

    void FoV();
    bool FoVLooking(sf::RectangleShape& boss_pos, std::vector<sf::Sprite> &walls);
    void Draw();
    void Move();
    void ChangeDirection();
    void Jump();
    void Gravity();
    void CheckCollision(std::vector<sf::Sprite> &blocks);
    void SetClock(float deltatime);
    bool CheckIfDead(std::vector<sf::Sprite>& blocks, std::vector<bool>& visible);
    sf::Vector2f GetPosition();
    sf::FloatRect GetGlobalBounds();
    void SetStartPosition(int map);
    bool moveFoV_onHit(const sf::RectangleShape& boss_rect);

    //FoV
    std::vector<sf::RectangleShape> FoV_vector;
    //this one controls the lenght
    const float size_of_fov = 800;
    const float number_of_rects = 100;
    sf::RectangleShape FoVpic;
    sf::Texture fovpic;
    //controll size of fov image
    const float width_of_fov_images = 3;
    bool started_FoV = false;
    bool left_or_right = false;
    //speed of FoV image
    const float speed_image_x = 600;
    const float speed_image_y = 75;
    //increase in width of FoV image
    const float Delta_width = 10;
    sf::Clock draw_fov;
    bool fovdraw = true;


    bool jumping;
    bool falling;

    //gravity speeds
    //gravity adjust
    //maxy max velocity for gravity
    const float maxY = 1000.f;
    //how much falling increases, the higher the faster fall
    const float gravityy = 1750;
    //max speed for left and right
    const float x_speed = 225;

    // Animation features
    bool charOnGround = false;
    bool charInAir = false;
    bool noKeyWasPressed = true;

    float time;

    enum class xDirectionID {RIGHT, LEFT, NONE};
    enum class yDirectionID {UP, DOWN, NONE};

    xDirectionID xDirection = xDirectionID::NONE;
    yDirectionID yDirection = yDirectionID::NONE;


    sf::RenderWindow* window;
    sf::RectangleShape character;

    //velocity x and y of character
    sf::Vector2f velocity;
    Configuration config;
};


#endif //MPAIN_PLAYER_H
