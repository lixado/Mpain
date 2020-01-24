#include <iostream>
#include <TGUI/TGUI.hpp>

#include "../Menu/stateMenu.h"
#include "../machine.h"
#include "stateSettings.h"

void backToMenu(sf::RenderWindow* window, Machine &machine, bool& close) {
    std::cout << "Back to menu..." << std::endl;
    machine.SetState(Machine::StateId::MENU);
    close = false;
}


void GodMode(Machine &machine)
{
    std::cout << "God mode..." << std::endl;

    //if god mode on set off ans opposite
    if(machine.returnConfigPointer()->GodMode)
    {
        machine.returnConfigPointer()->GodMode = false;
    } else
    {
        machine.returnConfigPointer()->GodMode = true;
    }

    std::cout << machine.returnConfigPointer()->GodMode << std::endl;
}

void StateSettings::GoNext(Machine &machine) {
    std::cout << "Settings starting..." << std::endl;
    close_loop = true;

    this->window_to_use = &machine.window;
    gui.setTarget(*window_to_use);


    try {
        load(machine);
    }
    catch (const tgui::Exception& ex){
        std::cerr << "Failed to load widgets: " << ex.what() << std::endl;
    }


    Update(machine);
}




void StateSettings::load(Machine &machine) {
    // Create the background image
    auto picture = tgui::Picture::create(config.settings_background);
    picture->setSize({"100%", "100%"});
    gui.add(picture);

    sf::Font font;

    if(!font.loadFromFile("Assets/SuperMario256.ttf")) {
        std::cout << "Failed to load font file. " << std::endl;
    }

    //Creates a volume slider.
    auto slider = tgui::Slider::create();

    //Sets relative position, to make the window reziseable.
    slider->setPosition("35%", "35%");
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setValue(machine.returnMusicPointer()->getVolume());
    slider->setSize("30%", "2%");
    gui.add(slider, "slider");
    pointer_slider = slider;

    auto sliderLabel= tgui::Label::create("Volume");
    sliderLabel->setPosition("40%", "30%");
    sliderLabel->setInheritedFont(font);
    sliderLabel->setTextSize(40);
    sliderLabel->getRenderer()->setTextColor(sf::Color::White);
    gui.add(sliderLabel);

    auto button = tgui::Button::create("Back to menu");
    button->setTextSize(0);
    button->setInheritedFont(font);
    button->setSize({btnConfig.width, btnConfig.height});
    button->setPosition({"35%", "30%"});
    button->setPosition(0, 0);
    button->getRenderer()->setTextColor(sf::Color::White);
    button->getRenderer()->setBorderColor(sf::Color::Transparent);
    button->getRenderer()->setBackgroundColor(sf::Color::Transparent);
    button->getRenderer()->setBackgroundColorHover(sf::Color::Transparent);
    button->getRenderer()->setBorderColorHover(sf::Color::Transparent);
    button->getRenderer()->setBackgroundColorDown(sf::Color::Transparent);
    button->getRenderer()->setBorderColorDown(sf::Color::Transparent);
    button->getRenderer()->setBorderColorFocused(sf::Color::Transparent);

    gui.add(button);


    button->connect("pressed", backToMenu, std::ref(window_to_use), std::ref(machine), std::ref(close_loop));


    auto checkbox = tgui::CheckBox::create();
    checkbox->setTextSize(0);
    checkbox->setSize({"4%", "4%"});
    checkbox->setPosition("61%", "50%");
    gui.add(checkbox);

    auto checkboxLabel = tgui::Label::create("God Mode");
    checkboxLabel->setTextSize(40);
    checkboxLabel->setInheritedFont(font);
    checkboxLabel->setPosition("36%", "49.5%");
    checkboxLabel->getRenderer()->setTextColor(sf::Color::White);
    gui.add(checkboxLabel);

    if(machine.returnConfigPointer()->GodMode)
    {
        checkbox.get()->setChecked(true);
    }else
    {
        checkbox.get()->setChecked(false);
    }

    checkbox->connect("checked", GodMode, std::ref(machine));
    checkbox->connect("unchecked", GodMode, std::ref(machine));


}


void StateSettings::Update(Machine &machine) {


    while (window_to_use->isOpen() && close_loop)
    {

        sf::Event event{};
        while (window_to_use->pollEvent(event))
        {

            // When the window is closed, the application ends
            if (event.type == sf::Event::Closed) {
                window_to_use->close();
                machine.SetRunning(false);

            }

            // When the window is resized, the view is changed
            else if (event.type == sf::Event::Resized)
            {
                window_to_use->setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
                gui.setView(window_to_use->getView());
            }

            if(pointer_slider->isFocused())
                machine.returnMusicPointer()->setVolume(pointer_slider->getValue());


            // Pass the event to all the widgets
            gui.handleEvent(event);
        }

        window_to_use->clear();

        // Draw all created widgets
        gui.draw();

        window_to_use->display();
    }

    //std::cout << "Exit" << std::endl;
}