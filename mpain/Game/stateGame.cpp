//
// Created by pedro on 10/16/19.
//

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "stateGame.h"
#include "../machine.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <unistd.h>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/TextBox.hpp>
#include <fstream>
#include "Load.h"
#include "Player.h"
#include "../Game/PlayerAnimation.h"
#include "Boss.h"
#include <chrono>
#include <iomanip>

void StateGame::GoNext(Machine &machine) {

    died_bool = false;
    current_map = 1;
    //Load all maps
    for (int i = 1; i < config.Maps.size() + 1; ++i) {
        //load map
        Load temp_load;

        if (!temp_load.loadMap(config.Maps[i - 1]))
            std::cout << "Failed to load map" << std::endl;

        Maps[i] = temp_load;

    }

    dead_character.loadFromFile("Assets/Images/Sprites/Character/dead_mario.png");

    //Stops the menu sound
    machine.returnMusicPointer()->stopMusic(config.menuMusic);

    //Plays map music
    machine.returnMusicPointer()->playMusic(config.map1Music, true);

    Load load = Maps[current_map];

    this->window_to_use = &machine.window;

    gui.setTarget(*window_to_use);

    auto viewFPS = tgui::Label::create();
    auto viewDeathCounter = tgui::Label::create();
    auto viewTime = tgui::Label::create();
    auto viewMap = tgui::Label::create();

    viewFPS_pointer = viewFPS;
    viewDeathCounter_pointer = viewDeathCounter;
    viewTime_pointer = viewTime;
    viewMap_pointer = viewMap;

    resetGameOverlay();
    GameOverlay();

    //make objects
    Player character(window_to_use);
    PlayerAnimation animationPlayer(character);
    Boss boss(window_to_use);
    sf::Event event;

    float deltatime = 0;

    fpsclock.restart();

    clock.restart().asSeconds();

    // Game loop (main loop)
    while (window_to_use->isOpen()) {

        //set time it took to render frame into character
        character.SetClock(deltatime);
        //print frames per second
        FramesPerSecond();

        // Clear window_to_use to black
        window_to_use->clear();

        // Event loop
        while (window_to_use->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                boss.boss_alive = false;
                usleep(100000);
                machine.SetRunning(false);
                window_to_use->close();
            }
            gui.handleEvent(event);
        }

        //extra functions for keys. Returns true if StateGame should close
        if (KeyPressedExtraFunctions(character, boss, machine))
            break;

        //All character interactions
        CharacterInteractions(character, load, machine, boss,  animationPlayer);

        //Lock funtion
        LockFunction(character, load);

        //False ground function
        FalseGroundFunction(character, load);

        //Draw objects
        DrawObjects(load, character);

        //Draw player
        animationPlayer.Draw(machine.window, character);

        //Functions for map8 boss
        Map8Functions(character, boss, load, deltatime, animationPlayer, machine);

        //Update game overlay
        updateGameOverlay();

        //Draw Game Overlay
        gui.draw();

        if (ChangeMap_Kill_on_Falling(character, load, machine, animationPlayer,boss))
            break;

        window_to_use->display();

        deltatime = clock.restart().asSeconds();
    }

    std::cout << "Exit game" << std::endl;
}


void StateGame::CharacterInteractions(Player &character, Load &load, Machine &machine, Boss &boss, PlayerAnimation& animationPlayer)
{
    //Change character direction
    character.ChangeDirection();
    //Character collision
    character.CheckCollision(load.tilesets_sprites);
    //Gravity
    character.Gravity();
    //Move character
    character.Move();

    //Jump
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && !character.jumping && !character.falling) {

        machine.returnMusicPointer()->playSound(config.jumpSoundEffect);
        character.yDirection = Player::yDirectionID::UP;
        character.jumping = true;
        character.velocity.y = 600;
    }

    //Jumping/falling functions
    if (character.jumping) {
        character.Jump();
    }
    if (character.falling) {
        machine.returnMusicPointer()->stopSound(config.jumpSoundEffect);
    }

    //check if dead
    if (character.CheckIfDead(load.death_sprites, load.death_sprites_show) ||
        boss.attack1.getGlobalBounds().intersects(character.character.getGlobalBounds())) {
        if (!machine.returnConfigPointer()->GodMode) {
            DiedFunction(load,character,machine,animationPlayer, boss);
        }
    }
    //check if dead for boss 8
    if (boss.attack3.getGlobalBounds().intersects(character.character.getGlobalBounds())) {
        if (boss.LaserIntersect(&character.character, &boss.attack3)) {
            if (!machine.returnConfigPointer()->GodMode) {
                DiedFunction(load,character,machine,animationPlayer, boss);
            }
        }
    }

}

void StateGame::DrawObjects(Load &load, Player &character) {
    //Background
    for (auto const &back : load.backgrounds)
        window_to_use->draw(back);
    //Background sprites
    for (auto const &back_blocks : load.background_sprites)
        window_to_use->draw(back_blocks);
    //Normal Sprites
    for (auto const &sprite : load.tilesets_sprites)
        window_to_use->draw(sprite);

    //Death sprites
    for (int i = 0; i < load.death_sprites.size(); i++) {
        if (load.death_sprites_show[i] == true) {
            window_to_use->draw(load.death_sprites[i]);
        }
    }

    //Falseground sprites
    for (int i = 0; i < load.falseground_sprites.size(); i++) {
        if (load.falseground_sprites_show[i] == true) {
            window_to_use->draw(load.falseground_sprites[i]);
        }
    }

    //Check if key is found, if not draw key
    if (!load.key_found) {
        window_to_use->draw(load.key_sprite);
    }
}

void StateGame::Map8Functions(Player &character, Boss &boss, Load &load, float &deltatime, PlayerAnimation& playerAnimation, Machine& machine)
{
    if (current_map == 8)
    {
        boss.Start(boss, character, load.wall_sprites, load.key_found, machine.returnMusicPointer());
        boss.BossMovement(load.tilesets_sprites, deltatime);
        boss.Draw(playerAnimation, deltatime);

        if (boss.hited_boss)
        {
            //if true its done
            character.moveFoV_onHit(boss.boss_rect);
        }

        if (!boss.boss_alive) {
            load.key_found = true;
            for (int i = 0; i < load.lock_sprite_positions.size(); i++) {
                load.tilesets_sprites[load.lock_sprite_positions[i]].setPosition(-500, -500);
            }
        }
    }

    character.FoV();

}

void StateGame::LockFunction(Player &character, Load &load) {
    //Check if player collided with key, remove lock if yes and remove key
    if (character.GetGlobalBounds().intersects(load.key_sprite.getGlobalBounds()) &&
        !load.lock_sprite_positions.empty()) {
        for (int i = 0; i < load.lock_sprite_positions.size(); i++) {
            load.tilesets_sprites[load.lock_sprite_positions[i]].setPosition(-500, -500);
        }
        load.key_found = true;
    }

}

void StateGame::FalseGroundFunction(Player &character, Load &load) {
    for(int i = 0; i < load.falseground_sprites.size(); i++)
    {
        if (character.GetGlobalBounds().intersects(load.falseground_sprites[i].getGlobalBounds()))
        {
            load.falseground_sprites_show[i] = false;
        }
    }

}

bool StateGame::ChangeMap_Kill_on_Falling(Player &character, Load &load, Machine &machine, PlayerAnimation& animationPlayer, Boss& boss) {
    //changing map
    if ((character.GetPosition().x + character.GetGlobalBounds().width) > config.screenWidth ||
        character.GetPosition().y < 0) {
        //set key_found to false
        load.key_found = false;

        current_map += 1;
        if (current_map > 9)
        {
            int time = timer_minutes*60 + timer_seconds;
            int deaths = deathCounter;

            std::string name;
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);

            std::ostringstream oss;
            oss << std::put_time(&tm, "%d-%m-%Y");
            name = oss.str();

            GameDataToFile(name, time, deaths);
            //change to scoreboard
            machine.returnMusicPointer()->stopMusic(config.map1Music);
            machine.returnMusicPointer()->playMusic(config.menuMusic, true);
            boss.boss_alive = false;
            usleep(400000);
            machine.SetState(Machine::StateId::SCOREBOARD);
            gui.removeAllWidgets();
            machine.GoNext();
            return true;
        }


        load = Maps[current_map];

        character.SetStartPosition(current_map - 1);
    }
    //Kill if fell down
    if ((character.GetPosition().y + character.GetGlobalBounds().height) > config.screenHeight) {
        DiedFunction(load,character,machine,animationPlayer, boss);

    }

    return false;
}

bool StateGame::KeyPressedExtraFunctions(Player &character, Boss &boss, Machine &machine) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        character.SetStartPosition(current_map - 1);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        machine.returnMusicPointer()->stopMusic(config.map1Music);
        machine.returnMusicPointer()->playMusic(config.menuMusic, true);
        boss.boss_alive = false;
        usleep(400000);
        machine.SetState(Machine::StateId::MENU);
        gui.removeAllWidgets();
        return true;
    }
    return false;
}

void StateGame::FramesPerSecond() {
    if (fpsclock.getElapsedTime().asMilliseconds() >= 1000) {
        FPS = frames;
        frames = 0;
        fpsclock.restart();
        timer_seconds++;

        if (timer_seconds >= 60) {
            timer_minutes++;
            timer_seconds = 0;
        }
    }
    frames += 1;
}

void StateGame::GameOverlay() {


    std::string fpsToString = std::to_string(int(FPS));
    std::string deathCounterToString = std::to_string(deathCounter);
    std::string mapToString = std::to_string(current_map);
    std::string minutesToString;
    std::string secondsToString;

    sf::Font font;
    if(!font.loadFromFile("Assets/SuperMario256.ttf")) {
        std::cout << "Failed to load font file. " << std::endl;
    }

    if(timer_minutes > 9)
        minutesToString = std::to_string(timer_minutes);

    if (timer_seconds > 9)
        secondsToString = std::to_string(timer_seconds);

    if (timer_minutes < 10)
        minutesToString = "0" + std::to_string(timer_minutes);

    if (timer_seconds < 10)
        secondsToString = "0" + std::to_string(timer_seconds);

    viewDeathCounter_pointer->getRenderer()->setFont(font);
    viewDeathCounter_pointer->getRenderer()->setTextColor(sf::Color::White);

#ifdef _WIN32
    viewDeathCounter_pointer->getRenderer()->setTextOutlineColor(sf::Color::Black);
    viewDeathCounter_pointer->getRenderer()->setTextOutlineThickness(5);
#endif

#ifdef linux
    viewDeathCounter_pointer->getRenderer()->setTextOutlineColor(sf::Color::Black);
    viewDeathCounter_pointer->getRenderer()->setTextOutlineThickness(5);
#endif

    viewDeathCounter_pointer->setTextSize(18);

    viewFPS_pointer = tgui::Label::copy(viewDeathCounter_pointer);
    viewTime_pointer = tgui::Label::copy(viewDeathCounter_pointer);
    viewMap_pointer = tgui::Label::copy(viewDeathCounter_pointer);

    viewFPS_pointer->setText("FPS: " + fpsToString);
    viewDeathCounter_pointer->setText("Deaths: " + deathCounterToString);
    viewTime_pointer->setText("Time: " + minutesToString + ":" + secondsToString);
    viewMap_pointer->setText("Map: "+ mapToString + " / 9");

    viewDeathCounter_pointer->setPosition(((gui.getView().getSize().x / 4.f)*1) - gui.getView().getSize().x/6.f, "0.5%");
    viewMap_pointer->setPosition(((gui.getView().getSize().x / 4.f)*2)  - gui.getView().getSize().x/6.f, "0.5%");
    viewTime_pointer->setPosition(((gui.getView().getSize().x / 4.f)*3) - gui.getView().getSize().x/6.f, "0.5%");
    viewFPS_pointer->setPosition(((gui.getView().getSize().x/4.f)*4) - gui.getView().getSize().x/6.f, "0.5%");

    gui.add(viewFPS_pointer);
    gui.add(viewDeathCounter_pointer);
    gui.add(viewTime_pointer);
    gui.add(viewMap_pointer);

}

void StateGame::updateGameOverlay() {

    std::string fpsToString = std::to_string(int(FPS));
    std::string deathCounterToString = std::to_string(deathCounter);
    std::string mapToString = std::to_string(current_map);
    std::string minutesToString;
    std::string secondsToString;

    if (timer_minutes > 9) minutesToString = std::to_string(timer_minutes);

    if (timer_seconds > 9) secondsToString = std::to_string(timer_seconds);


    if (timer_minutes < 10) minutesToString = "0" + std::to_string(timer_minutes);


    if (timer_seconds < 10) secondsToString = "0" + std::to_string(timer_seconds);

    viewFPS_pointer->setText("FPS: " + fpsToString);
    viewDeathCounter_pointer->setText("Deaths: " + deathCounterToString);
    viewTime_pointer->setText("Time: " + minutesToString + ":" + secondsToString);
    viewMap_pointer->setText("Map: "+ mapToString + " / 9");

}

void StateGame::resetGameOverlay() {
    timer_seconds = 0;
    timer_minutes = 0;
    FPS = 0;
    deathCounter = 0;
}

void StateGame::DiedFunction(Load& load, Player& character, Machine& machine, PlayerAnimation& animationPlayer, Boss& boss)
{
    machine.returnMusicPointer()->playSound(config.deathSoundEffect);
    deathCounter++;
    died_bool = true;
    died_clock.restart();

    character.velocity.y = 600;

    character.falling = false;
    character.jumping = true;

    while (died_clock.getElapsedTime().asSeconds() < 2)
    {

        FramesPerSecond();
        updateGameOverlay();
        //Draw objects
        DrawObjects(load, character);
        if(current_map == 8)
        {
            boss.Draw(animationPlayer, character.time);
        }
        //draw death image
        //Draw player
        character.character.setScale(1.6,1);
        character.character.setTexture(&dead_character);
        window_to_use->draw(character.character);


        character.Jump();
        character.Gravity();


        if (!character.jumping) {
            character.falling = true;
            character.velocity.y+=character.gravityy * character.time;
            character.character.setPosition(character.GetPosition().x, character.GetPosition().y+(character.velocity.y*clock.restart().asSeconds()));
        }

        //Draw Game Overlay
        gui.draw();
        window_to_use->display();

    }
    character.velocity.y = 0;
    character.character.setScale(1,1);
    died_bool = false;
    clock.restart();
    character.SetStartPosition(current_map - 1);
}

void StateGame::GameDataToFile(std::string date, int time, int deaths){
    int index;
    std::fstream m_file;
    m_file.open("gamedata.json", std::ios::in);
    Json::Value json_obj;
    // Factory to create char readers
    Json::CharReaderBuilder builder;
    Json::Value m_event;

    // Read data from file into root object
    std::string errors;
    bool parsingSuccessful = Json::parseFromStream(builder, m_file, &json_obj, &errors);
    if (!parsingSuccessful)
        std::cout << "Failed to read gamedata.json" << std::endl;
    m_file.close();
    index = m_event.size();
    m_event["date"] = date;
    m_event["deaths"] = deaths;
    m_event["time"] = time;

    // append to json object
    json_obj.append(m_event);

    //std::cout << json_obj.toStyledString() << std::endl;

    // write updated json object to file
    m_file.open("gamedata.json", std::ios::out);
    m_file << json_obj.toStyledString() << std::endl;
    m_file.close();
}
