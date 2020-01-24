#include <fstream>
#include <json/value.h>
#include <json/json.h>
#include "stateScoreboard.h"

void backToMenufromscore(sf::RenderWindow* window, Machine &machine, bool& close)
{
    //std::cout << "Back to menu..." << std::endl;
    machine.SetState(Machine::StateId::MENU);
    close = false;
}

void StateScoreboard::GoNext(Machine &machine) {
    //std::cout << "Scoreboard starting..." << std::endl;
    close_loop = true;

    this->window_to_use = &machine.window;
    gui.setTarget(*window_to_use);

    try {
        load(machine);
    }
    catch (const tgui::Exception& ex){
        std::cerr << "Failed to load widgets: " << ex.what() << std::endl;
    }

    StateScoreboard::readGameData();

    StateScoreboard::displayGameData(&gui, StateScoreboard::readGameData());

    Update(machine);
}



Json::Value StateScoreboard::readGameData(){
    //PARSE THE GAMEDATA FILE
    std::fstream m_file("gamedata.json");

    Json::Value json_obj;

    //Factory to create char readers
    Json::CharReaderBuilder builder;

    // Read data from file into root object
    std::string errors;
    bool parsingSuccessful = Json::parseFromStream(builder, m_file, &json_obj, &errors);

    if(!parsingSuccessful)
        std::cout << "Failed to read or is empty" << std::endl;


    m_file.close();



    return json_obj;
}

void resetScoreboard(Machine& machine, StateScoreboard* draw){
    std::ofstream file;
    file.open("gamedata.json", std::ofstream::out | std::ofstream::trunc);


    try {
        draw->load(machine);
    }
    catch (const tgui::Exception& ex){
        std::cerr << "Failed to load widgets: " << ex.what() << std::endl;
    }


    draw->displayGameData(&draw->gui, draw->readGameData());
}

void StateScoreboard::displayGameData(tgui::Gui *gui, Json::Value scores)
{
    Json::Value score = scores;

    //std::cout << score[0]["name"].asString() << std::endl;

    StateScoreboard::printer("date", "16.25%", score, gui);

    StateScoreboard::printer("deaths", "45%", score, gui);

    StateScoreboard::printer("time", "72.5%", score, gui);
}

bool compareInterval(Json::Value first, Json::Value second)
{
    return (first["deaths"].asInt() < second["deaths"].asInt());
}

Json::Value SortJson(Json::Value tosort)
{
    bool organized = false;

    while (!organized)
    {
        organized = true;
        for (int i = 0; i < tosort.size() - 1; ++i)
        {
            if(compareInterval(tosort[i+1], tosort[i]))
            {
                organized = false;
                Json::Value temp;
                temp = tosort[i];
                tosort[i] = tosort[i + 1];
                tosort[i + 1] = temp;
            }
        }
    }

    return tosort;
}

void StateScoreboard::printer(std::string topic, std::string xPosPercent, Json::Value score, tgui::Gui *gui)
{
    score = SortJson(score);

    for(int i = 0; i < 10; i++)
    {
        auto label = tgui::Label::create(score[i][topic].asString());
        label->setPosition(xPosPercent, 200 + 60 * i);
        label->setTextSize(36);
        label->getRenderer()->setTextColor(sf::Color::White);
        label->setInheritedFont(gui->getFont());
        gui->add(label);
    }
    //Draw column title in the end to not mess up the topic string
    topic[0] = toupper(topic[0]);
    auto label = tgui::Label::create(topic);
    label->setPosition(xPosPercent, 150);
    label->setTextSize(36);
    label->getRenderer()->setTextColor(sf::Color::White);
    label->setInheritedFont(gui->getFont());
    gui->add(label);
}


void StateScoreboard::load(Machine &machine) {

    sf::Font font;
    if(!font.loadFromFile("Assets/SuperMario256.ttf")) {
        std::cout << "Failed to load font file. " << std::endl;
    }
    gui.setFont(font);

    // Create the background image
    auto picture = tgui::Picture::create("Assets/Images/backgroundScoreboard.png");
    picture->setSize({"100%", "100%"});
    gui.add(picture);

    auto button = tgui::Button::create("Back to menu");
    button->setTextSize(0);
    button->setInheritedFont(font);
    button->setSize({"30%", "10%"});
    button->setPosition("0%", "0%");
    gui.add(button);

    button->getRenderer()->setTextColor(sf::Color::White);
    button->getRenderer()->setBorderColor(sf::Color::Transparent);
    button->getRenderer()->setBackgroundColor(sf::Color::Transparent);
    button->getRenderer()->setBackgroundColorHover(sf::Color::Transparent);
    button->getRenderer()->setBorderColorHover(sf::Color::Transparent);
    button->getRenderer()->setBackgroundColorDown(sf::Color::Transparent);
    button->getRenderer()->setBorderColorDown(sf::Color::Transparent);
    button->getRenderer()->setBorderColorFocused(sf::Color::Transparent);

    auto button1 = tgui::Button::copy(button);
    button1->setText("Reset Scoreboard");
    button1->setPosition("20","85%");
    gui.add(button1);


    button->connect("pressed", backToMenufromscore, std::ref(window_to_use), std::ref(machine), std::ref(close_loop));
    button1->connect("pressed", resetScoreboard,std::ref(machine), this);

}

void StateScoreboard::Update(Machine &machine) {

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

            // Pass the event to all the widgets
            gui.handleEvent(event);
        }

        window_to_use->clear();

        // READ GAME DATA AND DISPLAY //
        //readGameData(&gui);
        // Draw all created widgets
        gui.draw();

        window_to_use->display();
    }

    //std::cout << "Exit" << std::endl;
}