//
// Created by pedro on 10/25/19.
//

#ifndef MPAIN_LOAD_H
#define MPAIN_LOAD_H

#include <iostream>

#include <json/json.h>


#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <list>

struct Tileset
{
    unsigned int firstGid;
    int columns;
    int imageWidth;
    int imageHeight;
    int tileWidth;
    int tileHeight;
    int spacing;
    int tilecount;
    std::string name;

    sf::Texture texture;
};

class Load{
public:

    bool loadMap(const std::string& filename);
    void loadTileSheet(Json::Value &layer);
    void loadTileset(Json::Value& tileset);
    void loadTiles();
    void loadBackground(Json::Value& background);

    //variables
    std::vector<int> tileSheet;
    std::vector<sf::Sprite> tilesets_sprites;
    std::vector<sf::Sprite> death_sprites;
    std::vector<sf::Sprite> falseground_sprites;
    std::vector<bool> death_sprites_show;
    std::vector<bool> falseground_sprites_show;
    std::vector<sf::Sprite> background_sprites;
    //sprites walls map 8
    std::vector<sf::Sprite> wall_sprites;


    sf::Sprite key_sprite;
    bool key_found = false;
    std::vector<int> lock_sprite_positions;


    std::vector<std::shared_ptr<Tileset>> tilesets;
    std::vector<sf::Sprite> backgrounds;
    std::shared_ptr<sf::Texture> backgrounds_textures;




};


#endif //MPAIN_LOAD_H
