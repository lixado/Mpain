//
// Created by jakimi on 15.11.2019.
//

#include "../Game/PlayerAnimation.h"
#include "Boss.h"

PlayerAnimation::PlayerAnimation(Player &character)
{
    // Loads a spritesheet for the character
    if (!texture.loadFromFile("Assets/Images/Sprites/Character/LeftRight2.png")) {
        std::cout << "Failed to load player spritesheet!" << std::endl;
    }
    // Loads and checks boss frames
    if ((!bossLaserAttackTexture.loadFromFile("Assets/Images/Sprites/Boss/charizardAttack.png"))
        || (!bossLeftTexture.loadFromFile("Assets/Images/Sprites/Boss/charizardFront.png"))
        || (!bossRightTexture.loadFromFile("Assets/Images/Sprites/Boss/charizardFront(flipd).png"))
        || (!weak_texture.loadFromFile("Assets/Images/Sprites/Boss/charizardback.png")))
    {
        std::cout << "Failed to load boss spritesheet!" << std::endl;
    }

//make objects
    weak.setSpriteSheet(weak_texture);
    for (int i = 0; i < 7828; i+= 7828/51)
    {
        weak.addFrame(sf::IntRect(i, 0,7828/51 ,150));
    }


    // Intersect boss left frames
    flyingLeft.setSpriteSheet(bossLeftTexture);
    for(int i=0;i < 665 ; i+=665/5)
    {
        for (int j = 0; j < 1260; j+=1400/10)
        {
            flyingLeft.addFrame(sf::IntRect(i, j, 133, 140));
        }
    }
    // Intersect boss right frames
    flyingRight.setSpriteSheet(bossRightTexture);
    for(int i=0;i < 665 ; i+=665/5)
    {
        for (int j = 0; j < 1260; j+=1400/10)
        {
            flyingRight.addFrame(sf::IntRect(i, j, 133, 140));
        }
    }

    // Intersect boss attack frames
    shootingLaser.setSpriteSheet(bossLaserAttackTexture);
    for(int i=0;i < 960 ; i+=960/5)
    {
        for (int j = 0; j < 1760; j+=2304/12) // j < 2304; j += 2496/13
        {
            shootingLaser.addFrame(sf::IntRect(i, j, 192, 208));
        }
    }


    bossAnimeSprite.setFrameTime(sf::seconds(0.1));
    bossAnimeSprite.pause();
    bossAnimeSprite.setLooped(true);

    attack3 = false;


// walking left frames
    walkingLeft.setSpriteSheet(texture);
    walkingLeft.addFrame((sf::IntRect(175, 0, 29, 39))); // Standing still -l
    walkingLeft.addFrame(sf::IntRect(86, 0, 29, 39));
    walkingLeft.addFrame(sf::IntRect(113, 0, 29, 39));
    walkingLeft.addFrame(sf::IntRect(143, 0, 29, 39));

// walking right frames
    walkingRight.setSpriteSheet(texture);
    walkingRight.addFrame((sf::IntRect(203, 0, 29, 39))); // Standing still -r
    walkingRight.addFrame(sf::IntRect(235, 0, 29, 39));
    walkingRight.addFrame(sf::IntRect(262, 0, 29, 39));
    walkingRight.addFrame(sf::IntRect(294, 0, 29, 39));

// jump right frame
    jumpRight.setSpriteSheet(texture);
    jumpRight.addFrame(sf::IntRect(350, 0, 29, 39));

// jump left frame
    jumpLeft.setSpriteSheet(texture);
    jumpLeft.addFrame(sf::IntRect(27, 0, 29, 39));

// Starting animation when loaded on screen (right position)
    currentAnimation = &walkingRight;

// set up AnimatedSprite (duration of a frame, and looping)
    animeSprite.setFrameTime(sf::seconds(0.05));
    animeSprite.pause();
    animeSprite.setLooped(true);

// Scaling the player to appropriate size (ca. 2blocks)

    //they have Width 29 height 39

    // took + 10 to make size better
    animeSprite.setScale(((character.GetGlobalBounds().width*2))/ 29,(character.GetGlobalBounds().height + 10)/ 39);
}

//int 0 means flying, int 1 means attack 1, int 2 means attack 2, int 3 means attack 3, 4 is opener
void PlayerAnimation::BossDraw(sf::RenderWindow* window, sf::Vector2f pos, bool draw, int attack)
{
    sf::Time frameTimebossfloat = frameTimeboss.restart();

      if(attack == 3)
    {
        attack3 = true;
        attack3clock.restart();
        aniSpeed = 0.5;
    }
     //duration of attack 3 attack
    if (attack3clock.getElapsedTime().asMilliseconds() > 500)
    {
        bossAnimeSprite.pause();
        aniSpeed = 1;
        attack3 = false;
    }

    if (attack3)
    {
        bossCurrentAnimation = &shootingLaser;
    }else
    {
        bossCurrentAnimation = &flyingLeft;
    }


    bossAnimeSprite.play(*bossCurrentAnimation);

    bossAnimeSprite.setPosition(pos);

    // Update AnimatedSprite - deltaTime
    bossAnimeSprite.update(frameTimebossfloat * aniSpeed);

    if (draw)
    {
        // Draw/display on window
        window->draw(bossAnimeSprite);
    }

}

void PlayerAnimation::Draw(sf::RenderWindow& window, Player &character)
{
    // Checking if/which input is pressed and sets the appropriate animation/movement
    sf::Time frameTime = frameClock.restart();

    if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))) && character.charOnGround && !character.charInAir)
    {
        currentAnimation = &duckingLeft;
        character.noKeyWasPressed = false;
        sides = sidesID::LEFT;
    }
    if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))) && character.charOnGround && !character.charInAir)
    {
        currentAnimation = &duckingRight;
        character.noKeyWasPressed = false;
        sides = sidesID::RIGHT;
    }
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) )&& character.charOnGround && !character.charInAir)
    {
        currentAnimation = &walkingLeft;
        character.noKeyWasPressed = false;
        sides = sidesID::LEFT;
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) ) && character.charOnGround && !character.charInAir)
    {
        currentAnimation = &walkingRight;
        character.noKeyWasPressed = false;
        sides = sidesID::RIGHT;
    }
    if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && !character.charOnGround && character.charInAir)
        || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && !character.charOnGround && character.charInAir) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && sides == sidesID::RIGHT))
    {
        currentAnimation = &jumpRight;
        character.noKeyWasPressed = false;
        sides = sidesID::RIGHT;
    }
    if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)|| sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))&& !character.charOnGround && character.charInAir)
        || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && !character.charOnGround && character.charInAir) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && sides == sidesID::LEFT))
    {
        currentAnimation = &jumpLeft;
        character.noKeyWasPressed = false;
        sides = sidesID::LEFT;
    }

    animeSprite.play(*currentAnimation);

    // If no key was pressed set Standing-Still frame (left or right according to which was set in last)
    if (character.noKeyWasPressed)
    {
        animeSprite.stop();
    }
    character.noKeyWasPressed = true;

    // Display standing frame after a jump
    if(character.charOnGround && !character.charInAir && sides == sidesID::RIGHT && !(sides == sidesID::LEFT))
    {
        currentAnimation = &walkingRight;
    }
    if(character.charOnGround && !character.charInAir && sides == sidesID::LEFT && !(sides == sidesID::RIGHT))
    {
        currentAnimation = &walkingLeft;
    }

    animeSprite.setPosition(character.GetPosition().x - character.GetGlobalBounds().width/2+0.5, character.GetPosition().y - 10);


    // Update AnimatedSprite - deltaTime
    animeSprite.update(frameTime);

    // Draw/display on window
    window.draw(animeSprite);
}