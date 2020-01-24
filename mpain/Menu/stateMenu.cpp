#include <iostream>

#include "stateMenu.h"
#include "../machine.h"
#include "../Sound/LoadSound.h"


void startGame(sf::RenderWindow* window, Machine &machine) {
    //std::cout << "Starting game..." << std::endl;
    window->clear();
    window->setView(window->getDefaultView());
    machine.SetState(Machine::StateId::GAME);
    machine.GoNext();

}

void goToWindow(sf::RenderWindow* window, Machine &machine) {
    //std::cout << "Settings..." << std::endl;
    machine.SetState(Machine::StateId::SETTINGS);
    machine.GoNext();

}

void scoreboard(sf::RenderWindow* window, Machine &machine) {
    //std::cout << "Settings..." << std::endl;
    machine.SetState(Machine::StateId::SCOREBOARD);
    machine.GoNext();

}

void exitGame(sf::RenderWindow* window, Machine &machine) {
    //std::cout << "Exit game..." << std::endl;
    window->close();
    machine.SetRunning(false);
}

void StateMenu::GoNext(Machine &machine)
{
        this->window_to_use = &machine.window;

        machine.returnMusicPointer()->playMusic(config.menuMusic, true);

        //std::cout << "Menu starting..." << std::endl;
        gui.setTarget(machine.window);


        try {
            load(machine);
        }
        catch (const tgui::Exception& ex){
            std::cerr << "Failed to load widgets: " << ex.what() << std::endl;
        }

        Update(machine);
}

void StateMenu::load(Machine &machine) {

    // Create the background image
    // The picture is of type tgui::Picture::Ptr which is actually just a typedef for std::shared_widget<Picture>
    // The picture will fit the entire window and will scale with it
    auto picture = tgui::Picture::create("Assets/Images/menu.png");
    picture->setSize({"100%", "100%"});
    gui.add(picture);

    sf::Font font;

    if(!font.loadFromFile("Assets/SuperMario256.ttf")) {
        std::cout << "Failed to load font file. " << std::endl;
    }

    // Create buttons, first button is a template button
    auto button = tgui::Button::create("Start Game");
    button->setTextSize(0);
    button->setSize({btnConfig.width, btnConfig.height});
    button->setInheritedFont(font);
    button->getRenderer()->setTextColor(sf::Color::White);
    button->setPosition({"25%", "34%"});
    button->getRenderer()->setBorderColor(sf::Color::Transparent);
    button->getRenderer()->setBackgroundColor(sf::Color::Transparent);
    button->getRenderer()->setBackgroundColorHover(sf::Color::Transparent);
    button->getRenderer()->setBorderColorHover(sf::Color::Transparent);
    button->getRenderer()->setBackgroundColorDown(sf::Color::Transparent);
    button->getRenderer()->setBorderColorDown(sf::Color::Transparent);
    button->getRenderer()->setBorderColorFocused(sf::Color::Transparent);
    gui.add(button);

    auto button2 = tgui::Button::copy(button);
    button2->setText("Settings");
    button2->setSize({btnConfig.width, btnConfig.height});
    button2->setPosition({"25%", "49%"});
    gui.add(button2);

    auto button4 = tgui::Button::copy(button);
    button4->setText("Scoreboard");
    button4->setSize({btnConfig.width, btnConfig.height});
    button4->setPosition({"25%", "64%"});
    gui.add(button4);

    auto button3 = tgui::Button::copy(button);
    button3->setText("Exit game");
    button3->setSize({btnConfig.width, btnConfig.height});
    button3->setPosition("25%", "79%");
    gui.add(button3);


    // Call functions when buttons are pressed
    button->connect("pressed", startGame, std::ref(window_to_use), std::ref(machine));
    button2->connect("pressed", goToWindow, std::ref(window_to_use), std::ref(machine));
    button3->connect("pressed", exitGame, std::ref(window_to_use), std::ref(machine));
    button4->connect("pressed", scoreboard, std::ref(window_to_use), std::ref(machine));


}


void StateMenu::Update(Machine &machine) {

    while (window_to_use->isOpen())
    {

        sf::Event event;
        while (window_to_use->pollEvent(event)) {

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

            // Pass the event to all the widgets
            gui.handleEvent(event);
        }


        window_to_use->clear();

        // Draw all created widgets
        gui.draw();

        window_to_use->display();
    }

}