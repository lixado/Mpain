#ifndef STATEMENU_H
#define STATEMENU_H

#include "../state.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <TGUI/TGUI.hpp>

#include "../Configurations/Configuration.h"
#include "../Configurations/ButtonConfig.h"
#include "../Sound/SoundClip.h"
#include "../Sound/Music.h"

class StateMenu : public State {
public:

    void GoNext(Machine &machine);
    void Update(Machine &machine);


protected:
    Configuration config;
    ButtonConfig btnConfig;
    sf::RenderWindow* window_to_use;
    tgui::Gui gui;

    void load(Machine &machine);

};

#endif
