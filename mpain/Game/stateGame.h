#ifndef STATEEXIT_H
#define STATEEXIT_H

#include "../state.h"
#include "Load.h"
#include "Player.h"
#include "Boss.h"
#include "PlayerAnimation.h"
#include <SFML/System/Clock.hpp>
#include <map>
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/TextBox.hpp>

class StateGame : public State {
public:
    void GoNext(Machine &machine);
    void CharacterInteractions(Player& character, Load& load, Machine &machine, Boss& boss, PlayerAnimation& animationPlayer);
    void DrawObjects(Load& load, Player& character);
    void Map8Functions(Player& character, Boss& boss, Load& load, float& deltatime, PlayerAnimation& playerAnimation, Machine& machine);
    void LockFunction(Player& character, Load& load);
    void FalseGroundFunction(Player& character, Load& load);
    bool ChangeMap_Kill_on_Falling(Player& character, Load& load, Machine& machine, PlayerAnimation& animationPlayer, Boss& boss);
    bool KeyPressedExtraFunctions(Player& character, Boss& boss, Machine& machine);
    void GameDataToFile(std::string date, int time, int death);
    void FramesPerSecond();
    void GameOverlay();
    void updateGameOverlay();
    void resetGameOverlay();
    void DiedFunction(Load& load, Player& character, Machine& machine, PlayerAnimation& animationPlayer, Boss& boss);


protected:

    sf::Clock died_clock;
    bool died_bool = false;

    std::shared_ptr<tgui::Label> viewFPS_pointer;
    std::shared_ptr<tgui::Label> viewDeathCounter_pointer;
    std::shared_ptr<tgui::Label> viewTime_pointer;
    std::shared_ptr<tgui::Label> viewMap_pointer;

    sf::RenderWindow* window_to_use;
    tgui::Gui gui;
    sf::Clock clock;
    sf::Clock fpsclock;
    int current_map = 1;
    std::map<int , Load> Maps;

    int frames = 0;
    float FPS = 0;
    int deathCounter = 0;

    int timer_seconds;
    int timer_minutes;

    sf::Texture dead_character;

};

#endif
