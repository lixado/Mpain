//
// Created by Pedro on 04-Dec-19.
//

#ifndef MPAIN_STATESCOREBOARD_H
#define MPAIN_STATESCOREBOARD_H

#include "../state.h"
#include <TGUI/TGUI.hpp>
#include "../machine.h"

class StateScoreboard : public State{
public:
    void GoNext(Machine &machine);
    void Update(Machine &machine);

    Json::Value readGameData();
    void displayGameData(tgui::Gui *gui, Json::Value scores);
    void printer(std::string topic, std::string xPosPercent, Json::Value score, tgui::Gui *gui);
    //used to close loop
    bool close_loop = true;
    tgui::Gui gui;
    sf::RenderWindow* window_to_use;
    void load(Machine &machine);

protected:
    Configuration config;
};

#endif //MPAIN_STATESCOREBOARD_H
