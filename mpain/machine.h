#ifndef MACHINE_H
#define MACHINE_H

#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Configurations/Configuration.h"
#include "Game/GameData.h"

class State;

class Machine
{
public:
    enum class StateId { MENU , GAME, SETTINGS, SCOREBOARD};

    Machine();
    ~Machine();

    void GoNext();
    void SetState(StateId state);
    bool GetRunning() const { return running; }
    void SetRunning(bool running) { this->running = running; }
    GameData* returnMusicPointer(){ return &music;}
    sf::RenderWindow window;
    Configuration* returnConfigPointer(){ return &config;}

protected:
    Configuration config;
    GameData music;
    bool running;
    StateId state;
    std::map<StateId, State*> states;
};

#endif
