//
// Created by pedro on 10/25/19.
//

#include <fstream>
#include "Load.h"



bool Load::loadMap(const std::string &filename)
{
    tileSheet.clear();
    tilesets_sprites.clear();
    tilesets.clear();
    backgrounds.clear();
    death_sprites_show.clear();

    // Will contain the data we read in
    Json::Value root;

    // Stream used for reading the data file. The data file has been saved as JSON in Tiled
    std::ifstream file(filename);

    // Factory to create char readers
    Json::CharReaderBuilder builder;

    // Read data from file into root object
    std::string errors;
    bool parsingSuccessful = Json::parseFromStream(builder, file, &root, &errors);

    if (!parsingSuccessful)
        std::cout << "Failed to read json" << std::endl;


    // Read in each layer
    for (Json::Value& layer: root["layers"])
    {
        if (layer["type"].asString() == "tilelayer")
            loadTileSheet(layer);
        if (layer["type"].asString() == "imagelayer")
            loadBackground(layer);
    }

    // Load all tilesets
    for (Json::Value& tileset: root["tilesets"])
        loadTileset(tileset);

    loadTiles();

    return true;
}

void Load::loadTileSheet(Json::Value &layer)
{
    // Read in tilemap
    for (size_t i = 0; i < layer["data"].size(); i++)
        tileSheet.push_back(layer["data"][(int) i].asUInt());
}


void Load::loadBackground(Json::Value& background)
{
    sf::Sprite temp_background;
    auto ts = std::make_shared<sf::Texture>();

    ts->loadFromFile(background["image"].asString());

    backgrounds_textures = ts;

    temp_background.setTexture(*backgrounds_textures.get());
    temp_background.setPosition(background["offsetx"].asInt(),background["offsety"].asInt());


    backgrounds.push_back(temp_background);
}

void Load::loadTileset(Json::Value& tileset)
{
    auto ts = std::make_shared<Tileset>();

    ts->firstGid  = tileset["firstgid"].asUInt();
    ts->columns  = tileset["columns"].asInt();
    ts->imageWidth  = tileset["imagewidth"].asInt();
    ts->imageHeight  = tileset["imageheight"].asInt();
    ts->tileWidth  = tileset["tilewidth"].asInt();
    ts->tileHeight  = tileset["tileheight"].asInt();
    ts->spacing  = tileset["spacing"].asInt();
    ts->tilecount = tileset["tilecount"].asInt();
    ts->name = tileset["name"].asString();
    ts->texture.loadFromFile(tileset["image"].asString());

    tilesets.push_back(ts);
}

void Load::loadTiles()
{
    //for each square in tilesheet
    float xMultipliedBy = 0;
    float yMultipliedBy = 0;
    for (size_t i = 0; i < tileSheet.size(); ++i)
    {
        if (i == 30 * yMultipliedBy)
        {
            //Go further down on y-axis
            yMultipliedBy++;

            //Reset what x is multiplied by for every time it reaches the map width in tiles.
            xMultipliedBy = 0;
        }

        //if there is a tile
        if(tileSheet[i] != 0)
        {
            //find which corresponds to it
            for (auto const& tile : tilesets)
            {
                //if gid is this one
                //if number in data is bigger then firstgid and less then the tilecount then load this
                if (tileSheet[i] >= tile->firstGid && tileSheet[i] <= (tile->tilecount + tile->firstGid))
                {
                    //then its this tile
                    sf::Sprite temp_sprite;

                    int tileid = tileSheet[i] - tile->firstGid;
                    int x = (tileid % tile->columns) * (tile->tileWidth + tile->spacing);
                    int y = (tileid / tile->columns) * (tile->tileWidth + tile->spacing);

                    int rectWidth = tile->tileHeight;
                    int rectHeight = tile->tileWidth;

                    temp_sprite.setTexture(tile->texture);
                    temp_sprite.setTextureRect(sf::IntRect(x,y,rectWidth, rectHeight));
                    temp_sprite.setPosition(xMultipliedBy * 32, 32 * (yMultipliedBy - 1));



                    if (tile->name == "Death")
                    {
                        death_sprites.push_back(temp_sprite);
                        death_sprites_show.push_back(false);
                    }
                    else if(tile->name == "Background")
                    {
                        background_sprites.push_back(temp_sprite);
                    }
                    else if(tile->name == "Key")
                    {
                        key_sprite = temp_sprite;
                    }
                    else if(tile->name == "Lock")
                    {
                        lock_sprite_positions.push_back(tilesets_sprites.size());
                        tilesets_sprites.push_back(temp_sprite);
                    }
                    else if(tile->name == "falseground")
                    {
                        falseground_sprites.push_back(temp_sprite);
                        falseground_sprites_show.push_back(true);
                        
                    }
                    else
                    {
                        tilesets_sprites.push_back(temp_sprite);
                        if (tile->name == "Walls")
                        {
                            wall_sprites.push_back(temp_sprite);
                        }
                    }
                }
                //maybe else here

            }

        }


        //Go further in on the x-axis
        xMultipliedBy++;
    }

}