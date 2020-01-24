//
// Created by pedro on 11/1/19.
//

#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "Player.h"
#include "../Game/PlayerAnimation.h"

Player::Player(sf::RenderWindow* window)
{
    //controll size
    character.setSize(sf::Vector2f(14,34));
    character.setPosition(config.map_1_start);
    velocity.x = 0;
    velocity.y = 0;
    started_FoV = false;

    //make the rectangles
    for (int i = 0; i < number_of_rects; ++i)
    {
        sf::RectangleShape temp_rect;

        temp_rect.setFillColor(sf::Color::Red);
        temp_rect.setSize(sf::Vector2f(size_of_fov/number_of_rects, (i+1) * 2));
        temp_rect.setPosition(sf::Vector2f(i*(size_of_fov/number_of_rects), 100 - ((i+1) * 2) / 2) );
        FoV_vector.push_back(temp_rect);
    }
    fovpic.loadFromFile("Assets/Images/Sprites/Character/wave-FoV.png");

    FoVpic.setTexture(&fovpic, false);


    jumping = false;

    this->window = window;
}

void Player::FoV()
{
    for (int i = 0; i < number_of_rects; ++i)
    {
        if (xDirection == xDirectionID::RIGHT)
        {
            FoV_vector[i].setPosition(character.getPosition() + sf::Vector2f(i*(size_of_fov/number_of_rects) + character.getSize().x, (100 - ((i+1) * 2) / 2)-character.getSize().y*2 -character.getSize().y/2 - character.getSize().y/2 + 2));
        } else
        {
            //going left need to change
            FoV_vector[i].setPosition(character.getPosition() + sf::Vector2f((number_of_rects-i)*(size_of_fov/number_of_rects) - size_of_fov, (number_of_rects - ((i+1) * 2) / 2) - number_of_rects ));
        }

        //window->draw(FoV_vector[i]);
    }
}


//returns true if done, false if not
bool Player::moveFoV_onHit(const sf::RectangleShape& boss_rect)
{
    if (!started_FoV)
    {
        draw_fov.restart();
        //Set positions
        if (xDirection == xDirectionID::RIGHT)
        {
            left_or_right = true;
            //start size
            FoVpic.setSize(sf::Vector2f(FoV_vector[0].getSize().x * width_of_fov_images,FoV_vector[20].getSize().y));
            //set start position
            float midpoint_of_fov_rects = FoV_vector[FoV_vector.size()-1].getPosition().y + FoV_vector[FoV_vector.size()-1].getSize().y/2;
            FoVpic.setPosition(character.getPosition().x + character.getSize().x,midpoint_of_fov_rects - FoVpic.getSize().y/2);
            FoVpic.setRotation(0);
        } else
        {
            left_or_right = false;
            //set start size
            FoVpic.setSize(sf::Vector2f(FoV_vector[0].getSize().x * width_of_fov_images,FoV_vector[20].getSize().y));
            //set start position
            float midpoint_of_fov_rects = FoV_vector[FoV_vector.size()-1].getPosition().y + FoV_vector[FoV_vector.size()-1].getSize().y/2;
            FoVpic.setPosition(character.getPosition().x, midpoint_of_fov_rects + FoVpic.getSize().y/2);
            FoVpic.setRotation(180);
        }

        started_FoV = true;
    }
    //increase position and size by y
    //if right
    if (left_or_right)
    {
        FoVpic.setSize(sf::Vector2f(FoVpic.getSize().x + Delta_width * time, FoVpic.getSize().y + speed_image_y * time));
        FoVpic.setPosition(FoVpic.getPosition().x + speed_image_x * time, FoVpic.getPosition().y - (speed_image_y * time)/2);
    }
    //if left
    if (!left_or_right)
    {
        FoVpic.setSize(sf::Vector2f(FoVpic.getSize().x + Delta_width * time, FoVpic.getSize().y + speed_image_y * time));
        FoVpic.setPosition(FoVpic.getPosition().x - speed_image_x * time, FoVpic.getPosition().y + (speed_image_y * time)/2);
    }

    if (FoVpic.getGlobalBounds().intersects(boss_rect.getGlobalBounds()))
    {
        fovdraw = false;
    }

    // if max range started = false
    if (draw_fov.getElapsedTime().asSeconds() > 2)
    {
        fovdraw = true;
        started_FoV = false;
        return true;
    }

    //std::cout << "print" << std::endl;
    //check if didnt hit wall
    if (fovdraw)
    {
        window->draw(FoVpic);
    }
    return false;
}

bool Player::FoVLooking(sf::RectangleShape& boss_pos, std::vector<sf::Sprite> &walls)
{
    bool hit_wall = false;
    for (auto& wall : walls)
    {
        for (auto& rect : FoV_vector)
        {
            if (wall.getGlobalBounds().intersects(rect.getGlobalBounds()))
            {
                hit_wall = true;
            }
        }
    }

    //need to check for collision
    for (int i = 0; i < number_of_rects; ++i)
    {
        if (FoV_vector[i].getGlobalBounds().intersects(boss_pos.getGlobalBounds()) && !hit_wall)
        {
            //std::cout << "Hit boss" << std::endl;
            return true;
        }
    }
    return false;
}

void Player::SetStartPosition(int map)
{
    character.setPosition(config.Map_Start[map]);
}

sf::Vector2f Player::GetPosition()
{
    return character.getPosition();
}

sf::FloatRect Player::GetGlobalBounds()
{
    return character.getGlobalBounds();
}

bool Player::CheckIfDead(std::vector<sf::Sprite>& blocks, std::vector<bool>& visible)
{
    for(int i = 0; i < blocks.size(); i++)
    {
        if (character.getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
        {
            visible[i] = true;
            return true;
        }
    }
    return false;
}


void Player::CheckCollision(std::vector<sf::Sprite> &blocks)
{
    //The player should always be falling, as it is always pulled by gravity towards the ground, except for when jumping.
    if (!jumping) {
        falling = true;
    }

    //Reset velocity x for each time
    velocity.x = x_speed;


    //Coordinates for the player
    sf::FloatRect playerBounds = character.getGlobalBounds();

    //Coordinates to next x position
    sf::FloatRect xNext = playerBounds;

    if(xDirection == xDirectionID::RIGHT) {
        xNext.left = character.getPosition().x + 5;
        xNext.top = playerBounds.top;
    }
    if(xDirection==xDirectionID::LEFT) {
        xNext.left = character.getPosition().x - 5;
        xNext.top = playerBounds.top;
    }

    //Coordinates of next y position
    sf::FloatRect yNext = playerBounds;
    if(yDirection==yDirectionID::UP) {
        yNext.top = character.getPosition().y - 10;
        yNext.left = playerBounds.left;
    }
    if(yDirection==yDirectionID::DOWN) {
        yNext.top = character.getPosition().y + 10;
        yNext.left = playerBounds.left;
    }

    //Repeats for each object
    for(const auto& object : blocks)
    {

        //Coordinates for the object
        sf::FloatRect objectBounds = object.getGlobalBounds();

        //If player either collides with object over or under.
        if(yNext.intersects(objectBounds)) {
            if(yDirection==yDirectionID::DOWN) {

                //Set velocityY to 0, player can't move while colliding.
                velocity.y = 0.f;

                //Reposition player
                character.setPosition(character.getPosition().x, objectBounds.top - playerBounds.height);
                charOnGround = true;
                charInAir = false;
                falling = false;

            }
            else if(yDirection==yDirectionID::UP) {

                //Set velocityY to 0, player can't move while colliding.
                velocity.y = 0.f;

               //Reposition player
                //character.setPosition(character.getPosition().x, objectBounds.top + objectBounds.height);

                falling = true;
                jumping=false;

            }
        }

        //If player either collides with object right or left.
        if (xNext.intersects(objectBounds)) {
            if(xDirection==xDirectionID::RIGHT && !yNext.intersects(objectBounds)) {

                //Set velocityX to 0, player can't move while colliding.
                velocity.x = 0.f;


            }
            else if(xDirection==xDirectionID::LEFT && !yNext.intersects(objectBounds))
            {

                //Set velocityX to 0, player can't move while colliding.
                velocity.x = 0.f;

            }
        }
    }

}

void Player::Gravity()
{
    if(falling && !jumping)
    {
        charOnGround = false;

        //Change the player direction
        yDirection=yDirectionID::DOWN;

        //Add the gravity to velocity y for each tick
        velocity.y += gravityy * time;

        //If player reaches the maxY, player will move with maxY instead.
        if(velocity.y >= maxY) {
            velocity.y = maxY;
        }

        //Change the player position for each tick
        character.setPosition(character.getPosition().x, character.getPosition().y + (velocity.y * time));
    }
}

void Player::Draw()
{
    window->draw(character);
}

void Player::Move()
{

    // Handle input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
    {
        xDirection=xDirectionID::RIGHT;

        character.setPosition(character.getPosition().x + (velocity.x * time), character.getPosition().y);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        xDirection=xDirectionID::LEFT;

        character.setPosition(character.getPosition().x - (velocity.x * time), character.getPosition().y);
    }

    //Player can't go out of map on left side.
    character.setPosition(std::max(character.getPosition().x, 0.f), character.getPosition().y);
}

void Player::ChangeDirection() {

    // Handle input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        xDirection=xDirectionID::RIGHT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        xDirection=xDirectionID::LEFT;
    }
}


void Player::Jump()
{
    if (velocity.y > 0) {
        //Change the player direction
        yDirection = yDirectionID::UP;

        //Subtract the gravity from velocity y for each tick
        velocity.y -= gravityy * time;

        charOnGround = false;
        charInAir = true;

        //Change the player position for each tick
        character.setPosition(character.getPosition().x, character.getPosition().y - (velocity.y * time));
    } else {
        //If the player is not jumping, the vertical speed is less than zero
        yDirection = yDirectionID::NONE;
        jumping = false;
    }
}

void Player::SetClock(float deltatime)
{
    this->time = deltatime;
}