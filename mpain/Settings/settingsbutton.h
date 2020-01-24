#ifndef SETTINGSBUTTON_H
#define SETTINGSBUTTON_H

#include <string>

//Settings for TGUI buttons
class settingsbutton {

    public:
        //Dimensions, in percentages of the window size(see Configuration.h)
        std::string width = "30%";
        std::string height = "10%";
        int textSize = 0;

};

#endif //SETTINGSBUTTON_H
