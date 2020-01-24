cmake_minimum_required(VERSION 3.14)
project(mpain)

set(CMAKE_CXX_STANDARD 14)
file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY Maps DESTINATION ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

SET(WINDOWS_RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/recources.rc)

add_executable (
        mpain
        main.cpp
        machine.cpp
        machine.h
        state.h

        Menu/stateMenu.cpp Menu/stateMenu.h

        Game/stateGame.cpp Game/stateGame.h Game/stateGame.h Game/Load.h
        Game/Load.cpp Game/Player.h Game/Player.cpp Game/PlayerAnimation.cpp
        Game/GameData.cpp Game/GameData.h

        Sound/SoundClip.h Sound/SoundClip.cpp Sound/Music.cpp Sound/Music.h Sound/LoadSound.cpp Sound/LoadSound.h

        Configurations/Configuration.h Configurations/ButtonConfig.h

        Animation/Animation.h Animation/Animation.cpp Animation/AnimatedSprite.h Animation/AnimatedSprite.cpp

        Settings/stateSettings.cpp Settings/stateSettings.h Settings/settingsbutton.h

        Game/Boss.cpp Game/Boss.h

        Scoreboard/stateScoreboard.cpp Scoreboard/stateScoreboard.h recources.rc)

target_link_libraries (
        mpain
        sfml-graphics sfml-window sfml-system sfml-audio
        jsoncpp
        tgui
)

target_include_directories(mpain PUBLIC /usr/local/jsoncpp)
target_include_directories(mpain PUBLIC /usr/include/jsoncpp C:/dev/libs/jsoncpp/include/jsoncpp)
