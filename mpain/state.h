#ifndef STATE_H
#define STATE_H

#include "Configurations/Configuration.h"

#include "Game/GameData.h"

class Machine;

class State
{
public:
    State() {}
    virtual ~State() {};
    virtual void GoNext(Machine &context) = 0;
    Configuration config;
};

#endif
