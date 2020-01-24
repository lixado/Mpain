//
// Created by pedro on 11/27/19.
//

#ifndef MPAIN_BOSS_H
#define MPAIN_BOSS_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Player.h"
#include "PlayerAnimation.h"
#include "GameData.h"

class Boss {
public:
    //constructor loads all textures
    Boss(sf::RenderWindow* window);
    //The draw function
    void Draw(PlayerAnimation& playerAnimation, float deltatime);

    //bool if boss has started
    bool started = false;
    //function to run at start of boss encounter
    void Start(Boss& temp_boss, Player& temp_character, std::vector<sf::Sprite> &walls, bool& key_found, GameData* sound);

    //attack pattern and movement
    void BossMovement(std::vector<sf::Sprite> &blocks, float delta);
    static void AttackPattern(Boss* boss, Player* character);
    static void Attack1(const sf::Vector2f& char_pos, sf::RectangleShape* Attack1Rect, bool* Attack1Bool,const sf::RectangleShape& boss_pos, float* Dx, float* Dy, Boss* boss);
    static void Attack2(bool* Attack2Bool , Boss* boss, Player* character);
    static void Attack3(const Player* character, sf::RectangleShape* Attack3Rect, bool* Attack3Bool,const sf::RectangleShape& boss_pos,  Boss* boss);
    static void Opening(Boss* boss, Player* character);

    //rectanglehitbox og boss
    sf::RectangleShape boss_rect;

    //STA algorithm
    sf::Vector2f NormalizeVector(sf::Vector2f vector);
    sf::Vector2f Projecting(sf::Vector2f axis1, std::vector<sf::Vector2f> vertices);
    float dotProduct(sf::Vector2f vect_A, sf::Vector2f vect_B);
    bool LaserIntersect(sf::RectangleShape* rectangleA, sf::RectangleShape* rectangleB);
    bool Overlap(sf::Vector2f a, sf::Vector2f b);

    //returns pointers to use in static functions
    sf::RectangleShape* ReturnAttack1Rect(){ return &attack1;}
    bool* ReturnAttack1Bool(){ return &drawAttack1;}
    bool* ReturnAttack2Bool(){ return &drawAttack2;}
    sf::RectangleShape* ReturnAttack3Rect(){ return &attack3;}
    bool* ReturnAttack3Bool(){ return &drawAttack3;}
    bool* Returndrawloose_1(){ return &drawloose_1;}

    //sound
    GameData* sounds;

    //walls for map 8
    std::vector<sf::Sprite>* walls;

    //to help move the attack1 in right direction
    float Dx = 0;
    float Dy = 0;

    Boss* boss;
    Player* character;

    //boss life variables, and animation for loosing hp
    bool hited_boss = false;
    int boss_lives = 3;
    bool boss_alive = true;
    static void Draw_HP_loss(sf::Vector2f cords, bool* drawloose_1, Boss* boss);
    sf::Texture loose_1;
    sf::RectangleShape loose_1_rect;
    bool drawloose_1 = false;

    bool attacking = false;

    //textures for boss when doing attacks
    sf::Texture textureBoss;
    sf::Texture textureBossOpen;
    sf::Texture textureBossBlind;

    //attack 1 variables
    sf::Texture textureAttack1;
    bool drawAttack1 = false;
    sf::RectangleShape attack1;

    //attack 2 variables
    bool drawAttack2 = false;
    sf::RectangleShape attack2;
    sf::Texture textureAttack2;
    //shake effect
    bool up_down = true;
    //speed of shake
    const float speed_shake = 300;
    // the less the more it moves
    const float size_of_shake = 5;

    //attack 3 variables
    sf::Texture textureAttack3;
    bool drawAttack3 = false;
    sf::RectangleShape attack3;

    bool opener = false;

protected:
    //Movement variables, OpenZone is the zone where the boss can be inside of
    sf::RectangleShape Open_Zone;
    enum class DirectionID {RIGHT, LEFT, NONE, UP , DOWN};
    std::vector<DirectionID> directions;
    DirectionID Direction = DirectionID::NONE;
    //speed om which boss moves
    const float speed = 225;

    //clock used for movement function
    sf::Clock boss_clock;

    //window used for draw
    sf::RenderWindow* window;
};


#endif //MPAIN_BOSS_H
