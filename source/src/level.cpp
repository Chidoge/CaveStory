#include "../headers/level.h"
#include "../headers/graphics.h"
#include "../headers/tinyxml2.h"

#include <SDL2/SDL.h>

#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) : 
    _mapName(mapName),
    _spawnPoint(spawnPoint),
    _size(Vector2(0, 0))    
{
    this->loadMap(mapName, graphics);
}

void Level::loadMap(std::string mapName, Graphics &graphics) {
    // this->_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("assets/backgrounds/bkBlue.png"));
    // this->_size = Vector2(1280, 960);

    XMLDocument doc;
    std::stringstream ss;
    ss << "assets/maps/" << mapName << ".tmx";
    doc.LoadFile(ss.str().c_str());

    XMLElement* mapNode = doc.FirstChildElement("map");

    /* Get width and height of map */
    int width, height;
    mapNode->QueryIntAttribute("width", &width);
    mapNode->QueryIntAttribute("height", &height);
    this->_size = Vector2(width, height);

    /* Get width and height of each individual tile */
    int tileWidth, tileHeight;
    mapNode->QueryIntAttribute("tilewidth", &tileWidth);
    mapNode->QueryIntAttribute("tileheight", &tileHeight);
    this->_tileSize = Vector2(tileWidth, tileHeight);

    /* Load the tilesets */
    XMLElement* pTileset = mapNode->FirstChildElement("tileset");
    if (pTileset != NULL) {
        while (pTileset) {
            int firstGid;
            
            const char* source = pTileset->FirstChildElement("image")->Attribute("source");
            std::stringstream ss;
            ss << "assets/tilesets/" << source;
            pTileset->QueryIntAttribute("firstgid", &firstGid);

            SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
            this->_tileSets.push_back(Tileset(tex, firstGid));

            pTileset = pTileset->NextSiblingElement("tileset");
        }
    }

    /* Load layers */
    XMLElement* pLayer = mapNode->FirstChildElement("layer");
    if (pLayer != NULL) {
        while (pLayer) {
            /* Loading the data element */
            XMLElement* pData = pLayer->FirstChildElement("data");
            if (pData != NULL) {
                while (pData) {
                    /* Loading the tile element */
                    XMLElement* pTile = pData->FirstChildElement("tile");

                    if (pTile != NULL) {
                        int tileCounter = 0;

                        while (pTile) {
                            /* Build each individual tile here 
                                If gid is 0, no tile should be drawn
                            */
                            if (pTile->IntAttribute("gid") == 0) {
                                tileCounter++;
                                if (pTile->NextSiblingElement("tile")) {
                                    pTile = pTile->NextSiblingElement("tile");
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }

                            /* Get the tileset for this specific gid */
                            int gid = pTile->IntAttribute("gid");
                            Tileset tls;

                            for (int i = 0; i < this->_tileSets.size(); i++) {
                                if (this->_tileSets[i].FirstGid <= gid) {
                                    tls = this->_tileSets.at(i);
                                    break;
                                }
                            }

                            if (tls.FirstGid == -1) {
                                /* No tileset found for this gid */
                                tileCounter++;

                                if (pTile->NextSiblingElement("tile")) {
                                    pTile = pTile->NextSiblingElement("tile");
                                    continue;
                                }
                                else {
                                    break;
                                }
                            }

                            /* Get position of the tile in the level */
                            int xx = 0;
                            int yy = 0;
                            xx = tileCounter % width;
                            xx *= tileWidth;
                            yy += tileHeight * (tileCounter / width);
                            Vector2 finalTilePosition = Vector2(xx, yy);

                            /* Calculate the position of the tile in the tileset */
                            int tilesetWidth, tilesetHeight;
                            SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
                            int tsxx = gid % (tilesetWidth / tileWidth) - 1;
                            tsxx *= tileWidth;
                            int tsyy = 0;
                            int amt = (gid / (tilesetWidth / tileWidth));
                            tsyy = tileHeight * amt;
                            Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

                            /* Build the actual file and add it to the level's tile list */
                            Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
                            this->_tileList.push_back(tile);
                            tileCounter++;

                            pTile = pTile->NextSiblingElement("tile");
                        }
                    }

                    pData = pData->NextSiblingElement("data");
                }
            }
            pLayer = pLayer->NextSiblingElement("layer");
        }
    }
}

void Level::update(int elapsedTime) {}

void Level::draw(Graphics &graphics) {
    for (int i = 0; i < this->_tileList.size(); i++) {
        this->_tileList.at(i).draw(graphics);
    }
}
 
Level::~Level() {}