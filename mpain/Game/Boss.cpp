//
// Created by pedro on 11/27/19.
//

#include <iostream>
#include <unistd.h>
#include <thread>
#include <cmath>
#include <SFML/Graphics/Texture.hpp>
#include "Boss.h"
#include "Player.h"

//Constructor to load all images and initiate variables
Boss::Boss(sf::RenderWindow* window)
{
    //size of zone where boss is
    Open_Zone.setPosition(490,32);
    Open_Zone.setSize(sf::Vector2f(430,820));

    //pre load attacks
    textureAttack3.loadFromFile("Assets/Images/Sprites/Boss/LaserBlue.png");
    attack3.setTexture(&textureAttack3, false);

    textureAttack2.loadFromFile("Assets/Images/Sprites/Boss/Blind_charizard.png");
    attack2.setTexture(&textureAttack2, false);

    textureAttack1.loadFromFile("Assets/Images/Sprites/Boss/FireBall.png");
    attack1.setTexture(&textureAttack1, false);
    attack1.setSize(sf::Vector2f(40,40));

    //pre load boss textures
    textureBossOpen.loadFromFile("Assets/Images/Sprites/Boss/char_back.png");
    textureBoss.loadFromFile("Assets/Images/Sprites/Boss/charizard_normal.png");
    textureBossBlind.loadFromFile("Assets/Images/Sprites/Boss/charizard_explode.png");

    loose_1.loadFromFile("Assets/Images/Sprites/Boss/-1.1.png");
    loose_1_rect.setSize(sf::Vector2f(50,50));
    loose_1_rect.setTexture(&loose_1, true);

    attack2.setPosition(-20,-20);
    attack2.setSize(sf::Vector2f(1000,1000));


    boss_rect.setTexture(&textureBoss, false);

    boss_rect.setSize(sf::Vector2f(100,100));

    //add directions
    directions.push_back(Boss::DirectionID::RIGHT);
    directions.push_back(Boss::DirectionID::LEFT);
    directions.push_back(Boss::DirectionID::NONE);
    directions.push_back(Boss::DirectionID::DOWN);
    directions.push_back(Boss::DirectionID::UP);

    int direction_rand =  rand() % 5;
    Direction = directions[direction_rand];

    boss_alive = true;

    this->window = window;
}

//this runs on a thread
void Boss::AttackPattern(Boss* boss, Player* character)
{
    //std::cout << "attack" << std::endl;

    //start attack pattern and dont stop unless boss is dead
    while(boss->boss_alive)
    {
        Attack1(character->GetPosition(), boss->ReturnAttack1Rect(), boss->ReturnAttack1Bool(), boss->boss_rect, &boss->Dx, &boss->Dy, boss);

        Attack2(boss->ReturnAttack2Bool(), boss, character);

        Attack3(character, boss->ReturnAttack3Rect(), boss->ReturnAttack3Bool(), boss->boss_rect, boss);

        Opening(boss, character);
    }
    //std::cout << "Died" << std::endl;
}


//Basic Ball twords person
void Boss::Attack1(const sf::Vector2f& char_pos, sf::RectangleShape* Attack1Rect, bool* Attack1Bool,const sf::RectangleShape& boss_pos,float* Dx, float* Dy, Boss* boss)
{
    //set start position at midle of boss
    Attack1Rect->setPosition(boss_pos.getPosition().x + (boss_pos.getSize().x) / 2, boss_pos.getPosition().y + (boss_pos.getSize().y) / 2);

    //calculate where to throw
    float delta_x = char_pos.x - boss_pos.getPosition().x;
    float delta_y = boss_pos.getPosition().y - char_pos.y;
    //Dy and Dx used in draw function to move ball
    *Dx = delta_x / sqrt(delta_x*delta_x + delta_y*delta_y);
    *Dy = delta_y / sqrt(delta_x*delta_x + delta_y*delta_y);
    double theta_radians = atan2(delta_y, delta_x);

    double degrees = theta_radians * (180.0/3.141592653589793238463);
    Attack1Rect->setRotation(-degrees);

    //set draw true
    *Attack1Bool = true;
    boss->sounds->playSound("Assets/sound/Lion-Roar.wav");

    //how long to draw
    std::chrono::milliseconds intervalv2(10);
    for (int i = 0; i < 600 ; i++)
    {
        if (boss->boss_alive)
            std::this_thread::sleep_for(intervalv2);
    }

    Attack1Rect->setPosition(-1000, -1000);
    *Attack1Bool = false;
}


//Blind if looking
//attack 2
void Boss::Attack2(bool* Attack2Bool , Boss* boss, Player* character)
{
    //set texture for boss to this so u can see the attack is coming
    boss->boss_rect.setTexture(&boss->textureBossBlind, true);
    //wait a bit for time to react
    std::chrono::milliseconds interval(10);
    for (int i = 0; i < 25 ; i++)
    {
        if (boss->boss_alive)
            std::this_thread::sleep_for(interval);
    }
    //check if looking for 100*10
    for (int i = 0; i < 200 ; i++)
    {
        if(character->FoVLooking(boss->boss_rect, *boss->walls))
        {
            //set true if looked
            *Attack2Bool = true;
        }
        if (boss->boss_alive)
        {
            std::this_thread::sleep_for(interval);
        }
    }

    //if looking
    if(*Attack2Bool)
    {
        boss->sounds->playSound("Assets/sound/Flashbang.wav");
        //blind for this time
        std::chrono::milliseconds interval2(10);
        for (int i = 0; i < 200 ; i++)
        {
            if (boss->boss_alive)
                std::this_thread::sleep_for(interval2);
        }
    }
    *Attack2Bool = false;
    boss->boss_rect.setTexture(&boss->textureBoss, true);
}


//attack 3
//laser attack
void Boss::Attack3(const Player* character, sf::RectangleShape* Attack3Rect,bool* Attack3Bool, const sf::RectangleShape& boss_pos , Boss* boss)
{
    sf::Vector2f position_with_delay(boss_pos.getPosition().x + (boss_pos.getSize().x) / 2, boss_pos.getPosition().y + (boss_pos.getSize().y) / 2);

    //calculate where to throw
    float delta_x = (character->character.getPosition().x + (character->character.getSize().x)/2) - (boss_pos.getPosition().x + (boss->boss_rect.getSize().x)/2);
    float delta_y = (boss_pos.getPosition().y + (boss->boss_rect.getSize().y)/2) - (character->character.getPosition().y + (character->character.getSize().y)/2);
    double theta_radians = atan2(delta_y, delta_x);
    double degrees = theta_radians * (180.0/3.141592653589793238463);

    //how long to draw
    *Attack3Bool = true;


    //delay for position
    std::chrono::milliseconds interval(10);
    for (int i = 0; i < 70 ; i++)
    {
        if (boss->boss_alive)
            std::this_thread::sleep_for(interval);
    }

    boss->sounds->playSound("Assets/sound/Lion-Roar.wav");


    Attack3Rect->setPosition(position_with_delay);
    Attack3Rect->setRotation(-degrees);
    //Calculate pytagores to find what size this should be
    Attack3Rect->setSize(sf::Vector2f(sqrt(delta_x*delta_x  + delta_y*delta_y  ) ,20));


    std::chrono::milliseconds intervalv2(10);
    for (int i = 0; i < 250 ; i++)
    {
        if (boss->boss_alive)
            std::this_thread::sleep_for(intervalv2);
    }

    Attack3Rect->setPosition(-1000, -1000);
    *Attack3Bool = false;
}

//function for boss opening so that u can hit boss
void Boss::Opening(Boss* boss, Player* character)
{
    boss->hited_boss = false;
    boss->opener = true;
    //*hited_boss = false;
    boss->boss_rect.setTexture(&boss->textureBossOpen, true);
    //2 seconds delay
    std::chrono::milliseconds intervalv2(10);
    for (int i = 0; i < 200 ; i++)
    {
        //check if character looked at boss
        if(character->FoVLooking(boss->boss_rect,*boss->walls) && !boss->hited_boss)
        {
            boss->hited_boss = true;
            boss->boss_lives -= 1;
            //run function of -1 HP on a diferent thread
            std::thread Draw_HP_loss(Boss::Draw_HP_loss, boss->boss_rect.getPosition(), boss->Returndrawloose_1(), boss);
            Draw_HP_loss.detach();
        }
        //Kill boss if lives = 0
        if (boss->boss_lives == 0)
        {
            boss->boss_alive = false;

        }
        if (boss->boss_alive)
            std::this_thread::sleep_for(intervalv2);
    }
    //set texture back to normal
    boss->opener = false;
    boss->boss_rect.setTexture(&boss->textureBoss, true);
    boss->hited_boss = false;
}

//Function
void Boss::Draw_HP_loss(sf::Vector2f cords, bool* drawloose_1, Boss* boss)
{
    //draw loose HP animation
    *drawloose_1 = true;
    boss->loose_1_rect.setPosition(cords);
    //rotate here
    std::chrono::milliseconds intervalv2(100);
    for (int i = 0; i < 20 ; i++)
    {
        if (boss->boss_alive)
            std::this_thread::sleep_for(intervalv2);
        //rotate the -1 animation and move it up
        boss->loose_1_rect.setPosition(boss->loose_1_rect.getPosition().x + 3 , boss->loose_1_rect.getPosition().y - 5);
        boss->loose_1_rect.rotate(2);
    }
    boss->loose_1_rect.setPosition(sf::Vector2f(-5000, -5000));
    *drawloose_1 = false;

}

//Controls movement of boss
void Boss::BossMovement(std::vector<sf::Sprite> &blocks, float delta)
{
    //if clock is 2 then we reset every x seconds
    //get new direction
    if (boss_clock.getElapsedTime().asSeconds() >= 2)
    {
        //set random direction
        int direction_rand =  rand() % 5;
        Direction = directions[direction_rand];
        boss_clock.restart();
    }

    //check if boss goes outside of the map
    //if yes set position to move inside the box
    if (boss_rect.getPosition().x + boss_rect.getSize().x >= 920)
    {
        Direction = DirectionID::LEFT;
    }
    if (boss_rect.getPosition().x <= 490)
    {
        Direction = DirectionID::RIGHT;
    }
    if (boss_rect.getPosition().y + boss_rect.getSize().y >= 852)
    {
        Direction = DirectionID::UP;
    }
    if (boss_rect.getPosition().y <= 32)
    {
        Direction = DirectionID::DOWN;
    }

    //move depending on direction
    if (Direction == DirectionID::DOWN)
    {
        boss_rect.setPosition(boss_rect.getPosition().x, boss_rect.getPosition().y + (speed * delta));
    }
    if (Direction == DirectionID::UP)
    {
        boss_rect.setPosition(boss_rect.getPosition().x, boss_rect.getPosition().y - (speed * delta));
    }
    if (Direction == DirectionID::RIGHT)
    {
        boss_rect.setPosition(boss_rect.getPosition().x + (speed * delta), boss_rect.getPosition().y);
    }
    if (Direction == DirectionID::LEFT)
    {
        boss_rect.setPosition(boss_rect.getPosition().x - (speed * delta), boss_rect.getPosition().y);
    }
}

//runs when character enters map 8
void Boss::Start(Boss& temp_boss, Player& temp_character, std::vector<sf::Sprite> &walls, bool& key_found, GameData* sound)
{
    if(!started)
    {
        //set key_found to false in case its true
        key_found = false;
        //start clock
        boss_clock.restart();
        started = true;

        //get walls
        this->walls = &walls;
        this->boss = &temp_boss;
        this->character = &temp_character;
        this->sounds = sound;

        //set random position
        boss_rect.setPosition(rand() % 920 - 490,rand() % 852 - 32);

        //start thread of attack pattern
        std::thread AttackPattern_thread(Boss::AttackPattern, this->boss, this->character);
        AttackPattern_thread.detach();
    }

}

//Draws if bool to the attack is true
void Boss::Draw(PlayerAnimation& playerAnimation, float deltatime)
{
    bool drawanime = true;
    if (drawAttack1)
    {
        drawanime = true;

        //Moves attack 1
        attack1.setPosition(attack1.getPosition().x + Dx *deltatime *500,attack1.getPosition().y - Dy * deltatime *500);

        window->draw(attack1);
    }
    if (drawAttack2)
    {
        drawanime = false;
        //make character blind

        //shake
        if (up_down)
        {
            attack2.setPosition(attack2.getPosition().x,attack2.getPosition().y - speed_shake * deltatime);
            if (attack2.getPosition().y < -20 - size_of_shake)
            {
                up_down = false;
            }
        }
        if (!up_down)
        {
            attack2.setPosition(attack2.getPosition().x,attack2.getPosition().y + speed_shake * deltatime);
            if (attack2.getPosition().y > -20 + size_of_shake)
            {
                up_down = true;
            }
        }


        window->draw(attack2);
    }
    if (drawAttack3)
    {
        drawanime = true;

        window->draw(attack3);
    }
    if(opener)
    {
        drawanime = false;
    }
    if (!drawanime && !drawAttack2)
    {
        window->draw(boss_rect);
    }

    if (boss_alive)
    {
        //window->draw(boss_rect);

        if (drawAttack3)
        {
            playerAnimation.BossDraw(window, boss_rect.getPosition(), drawanime, 3);
        } else
        {
            playerAnimation.BossDraw(window, boss_rect.getPosition(), drawanime, 0);
        }
        //draw -1 animation if boss loose HP
        if (drawloose_1)
        {
            window->draw(loose_1_rect);
        }
    }
}

bool Boss::LaserIntersect(sf::RectangleShape* rectangleA, sf::RectangleShape* rectangleB)
{
    //get all vertecis of rectangle
    std::vector<sf::Vector2f> vertices_char;
    vertices_char.clear();
    vertices_char.emplace_back(rectangleA->getTransform().transformPoint(0.f, 0.f));
    vertices_char.emplace_back(rectangleA->getTransform().transformPoint(rectangleA->getSize().x, 0.f));
    vertices_char.emplace_back(rectangleA->getTransform().transformPoint(rectangleA->getSize().x, rectangleA->getSize().y));
    vertices_char.emplace_back(rectangleA->getTransform().transformPoint(0.f, rectangleA->getSize().y));

    //get all vertecis of rectangle
    std::vector<sf::Vector2f> vertices_attack;
    vertices_attack.clear();
    vertices_attack.emplace_back(rectangleB->getTransform().transformPoint(0.f, 0.f));
    vertices_attack.emplace_back(rectangleB->getTransform().transformPoint(rectangleB->getSize().x, 0.f));
    vertices_attack.emplace_back(rectangleB->getTransform().transformPoint(rectangleB->getSize().x, rectangleB->getSize().y));
    vertices_attack.emplace_back(rectangleB->getTransform().transformPoint(0.f, rectangleB->getSize().y));

    //storage for axes
    std::vector<sf::Vector2f> axes1;
    std::vector<sf::Vector2f> axes2;

    //this has to change if its not rectangles
    int axes = 2;

    //get axes 1
    for (int i = 0; i < axes; i++)
    {
        //get first vertex
        sf::Vector2f p1 = vertices_char[i];
        // get the next vertex
        sf::Vector2f p2 = vertices_char[i + 1 == vertices_char.size() ? 0 : i+1 ];
        // subtract the two to get the edge vector
        sf::Vector2f edge = p1 - p2;
        // get either perpendicular vector
        sf::Vector2f normal(-edge.y, edge.x);
        axes1.push_back(NormalizeVector(normal));
    }

    //get axes 2
    for (int i = 0; i < axes; i++)
    {
        //get first vertex
        sf::Vector2f p1 = vertices_attack[i];
        // get the next vertex
        sf::Vector2f p2 = vertices_attack[i + 1 == vertices_attack.size() ? 0 : i+1];
        // subtract the two to get the edge vector
        sf::Vector2f edge = p1 - p2;
        // get either perpendicular vector
        sf::Vector2f normal(-edge.y, edge.x);
        axes2.push_back(NormalizeVector(normal));
    }


    //min and max storage
    std::vector<sf::Vector2f> min_max_1(2);
    std::vector<sf::Vector2f> min_max_2(2);

    //loop over the axes1
    for (int i = 0; i < axes1.size(); i++)
    {
        min_max_1[0] = Projecting(axes1[i], vertices_attack);
        min_max_1[1] = Projecting(axes1[i], vertices_char);
        if (!Overlap(min_max_1[0], min_max_1[1]))
        {
            // then we can guarantee that the shapes do not overlap
            return false;
        }

    }
    // loop over the axes2
    for (int i = 0; i < axes2.size(); i++)
    {
        min_max_2[0] = Projecting(axes2[i], vertices_attack);
        min_max_2[1] = Projecting(axes2[i], vertices_char);
        if (!Overlap(min_max_2[0], min_max_2[1]))
        {
            // then we can guarantee that the shapes do not overlap
            return false;
        }
    }
    //make true if hit
    //std::cout << "HITTTTTTTT" << std::endl;
    return true;
}

//returns the normalization of a vector and returns a 0,0 vector if recives a 0,0 vector
sf::Vector2f Boss::NormalizeVector(sf::Vector2f vector)
{
    float hypotenus_lenght = sqrt((vector.x*vector.x) + (vector.y*vector.y));
    sf::Vector2f temp;
    if (hypotenus_lenght == 0)
    {
        return sf::Vector2f(0,0);
    }
    temp = vector/hypotenus_lenght;
    return  temp;
}

//find the smallest point projected on the axis and the biggest point and return then
sf::Vector2f Boss::Projecting(sf::Vector2f axes1, std::vector<sf::Vector2f> vertices)
{
    //calculate dot product
    float min = dotProduct(axes1, vertices[0]);
    float max = min;

    for (int j = 1; j < vertices.size(); ++j)
    {
        float p = dotProduct(axes1, vertices[j]);
        if (p < min)
        {
            min = p;
        } else if (p > max)
        {
            max = p;
        }
    }
    return sf::Vector2f(min, max);
}

//return true if overlap otherwise false
bool Boss::Overlap(sf::Vector2f a, sf::Vector2f b)
{
    return (std::max((float)0, std::min(a.y, b.y) - std::max(a.x, b.x)));
}

//returns dot product of 2 vectors
float Boss::dotProduct(sf::Vector2f vect_A, sf::Vector2f vect_B)
{
    return (vect_A.x * vect_B.x + vect_A.y * vect_B.y);
}
