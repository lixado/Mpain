#ifndef STATESETTINGS_H
#define STATESETTINGS_H

#include "../state.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <TGUI/TGUI.hpp>

#include "../Configurations/Configuration.h"
#include "settingsbutton.h"
#include "../Sound/SoundClip.h"
#include "../Sound/Music.h"

class StateSettings : public State {
public:
    void GoNext(Machine &machine);
    void Update(Machine &machine);

    bool* Returnpointer(){ return &close_loop;}

    //used to close loop
    bool close_loop = true;

protected:
    Configuration config;
    settingsbutton btnConfig;
    sf::RenderWindow* window_to_use;
    tgui::Gui gui;
    void load(Machine &machine);
    std::shared_ptr<tgui::Slider> pointer_slider;
};

#endif