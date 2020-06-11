#include "../headers/level.h"
#include "../headers/graphics.h"
#include "../headers/tinyxml2.h"
#include "../headers/utils.h"

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

Level::~Level() {}

void Level::loadMap(std::string mapName, Graphics &graphics) {
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
            this->_tilesets.push_back(Tileset(tex, firstGid));

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

                            for (int i = 0; i < this->_tilesets.size(); i++) {
                                if (this->_tilesets[i].FirstGid <= gid) {
                                    tls = this->_tilesets.at(i);
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

    /* Parse collisions */
    XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
    if (pObjectGroup != NULL) {
        while (pObjectGroup) {
            const char* name = pObjectGroup->Attribute("name");
            std::stringstream ss;
            ss << name;

            if (ss.str() == "collisions") {
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        float x, y, width, height;
                        x = pObject->FloatAttribute("x");
                        y = pObject->FloatAttribute("y");
                        width = pObject->FloatAttribute("width");
                        height = pObject->FloatAttribute("height");
                        this->_collisionRects.push_back(Rectangle(
                            std::ceil(x) * globals::SPRITE_SCALE,
                            std::ceil(y) * globals::SPRITE_SCALE,
                            std::ceil(width) * globals::SPRITE_SCALE,
                            std::ceil(height) * globals::SPRITE_SCALE
                        ));

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }
            else if (ss.str() == "slopes") {
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        std::vector<Vector2> points;
                        Vector2 p1;
                        p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));

                        XMLElement* pPolyline = pObject->FirstChildElement("polyline");
                        if (pPolyline != NULL) {
                            std::vector<std::string> pairs;
                            const char* pointString = pPolyline->Attribute("points");

                            std::stringstream ss;
                            ss << pointString;
                            Utils::split(ss.str(), pairs, ' ');
                            
                            for (int i = 0; i < pairs.size(); i++) {
                                std::vector<std::string> ps;

                                Utils::split(pairs[i], ps, ',');
                                points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
                            }
                        }

                        for (int i = 0; i < points.size(); i+=2) {
                            this->_slopes.push_back(Slope(
                                Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
                                        (p1.y + points.at(i < 2 ? i : i - 1).y) * globals::SPRITE_SCALE),
                                Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
                                        (p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE)
                            ));
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }
            /* Other objectgroups go here */
            else if (ss.str() == "spawn points") {
                XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        float x = pObject->FloatAttribute("x");
                        float y = pObject->FloatAttribute("y");
                        const char* name = pObject->Attribute("name");
                        std::stringstream ss;
                        ss << name;

                        if (ss.str() == "player") {
                            this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }

            pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
        }
    }
}

void Level::update(int elapsedTime) {}

void Level::draw(Graphics &graphics) {
    for (int i = 0; i < this->_tileList.size(); i++) {
        this->_tileList.at(i).draw(graphics);
    }
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
    std::vector<Rectangle> others;

    for (int i = 0; i < this->_collisionRects.size(); i++) {
        if (this->_collisionRects[i].collidesWith(other)) {
            others.push_back(this->_collisionRects.at(i));
        }
    }

    return others;
}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
    std::vector<Slope> others;

    for (int i = 0; i < this->_slopes.size(); i++) {
        if (this->_slopes.at(i).collidesWith(other)) {
            others.push_back(this->_slopes.at(i));
        }
    }

    return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
    return this->_spawnPoint;
}