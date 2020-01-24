//
// Created by pedro on 10/17/19.
//

#include "machine.h"
#include "state.h"
#include "Menu/stateMenu.h"
#include "Game/stateGame.h"
#include "Settings/stateSettings.h"
#include "Scoreboard/stateScoreboard.h"


Machine::Machine()
{
    // Start the machine running
    running = true;

    //set up window
    window.create(sf::VideoMode(config.screenWidth, config.screenHeight), config.name);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(400);

    // Start in the hello state
    state = StateId::MENU;

    // Add states to the state map
    states.emplace(StateId::MENU, new StateMenu());
    states.emplace(StateId::GAME, new StateGame());
    states.emplace(StateId::SETTINGS, new StateSettings());
    states.emplace(StateId::SCOREBOARD, new StateScoreboard());
    sf::Image icon;
    // Application icon
    icon.loadFromFile("Assets/Images/ICO-mpain.jpg");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

}

Machine::~Machine()
{
    // Free the states from memory
    for (auto tempstate: states)
        delete tempstate.second;

    states.clear();
}

void Machine::GoNext()
{
    states[state]->GoNext(*this);
}

void Machine::SetState(StateId tempstate)
{
    this->state = tempstate;
}
