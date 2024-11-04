#include "level.h"
#include "graphics.h"
#include "globals.h"

#include <SDL.h>
#include "tinyxml2.h"
#include "utils.h"

#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Graphics& graphics) :
	_mapName(mapName),
	_size(Vector2(0, 0)) 
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {

}

void Level::loadMap(std::string mapName, Graphics& graphics) {
	//Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "Content/maps/" << mapName << ".tmx"; //pass in Map 1, we get maps/Map 1.tmx
	doc.LoadFile(ss.str().c_str()); //pass in ss convert to string to cstring
	
	XMLElement* mapNode = doc.FirstChildElement("map");  //gets the first child element called "map"

	//Get the width and the height of the whole map and store it within _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width); //from the first child element we look for the "width" property's int contained
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	//Get the width ad the height of the tiles and store it in the _tileSize variable
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	//load the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");//gets the first child element called "tileset"

	//since there may be multiple tilesets, we want to loop through and get all of them
	if (pTileset!=NULL) {
		while (pTileset) { 
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			char* path;
			std::stringstream ss;
			ss << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRender(), graphics.loadImage(ss.str()));
			this->_tilesets.push_back(Tileset(tex, firstgid));
			
			//get all the animations for that tileset
			XMLElement* pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL) {
				//first loop accesses all animations from all tilesets
				while (pTileA) {
					AnimatedTileInfo ati;
					//get the specific tileset's animations
					//we add firstgid to the tileId since our animations 
					//don't follow tilesets
					ati.StartTileId = pTileA->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;

					XMLElement* pAnimation = pTileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						//this loop accesses the animations in a tileset
						while (pAnimation) {
							XMLElement* pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									//put the frames into our vector of frames
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->_animatedTileInfos.push_back(ati);
					pTileA = pTileA->NextSiblingElement("tile");
				}
			}
			
			//loops us to the next element. think linkedlist
			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}
	//load the layers of the tiles
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			//loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//build each individual tile here
							//if gid is 0, no tile is drawn, continue loop
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									//we put this here since if there is no tile, we can't move forwards
									pTile = pTile->NextSiblingElement("tile");
									//immediately loops us
									continue;
								}
								else {
									//this is when we have no more tiles, we break out of the loop
									break;
								}
							}

							//Get the tileset for specific gids
							//we want to use the tileset that is lesser than the gid number and closest to it.
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									if (this->_tilesets[i].FirstGid > closest) {
										closest = this->_tilesets[i].FirstGid;
										tls = this->_tilesets.at(i);
									}
								}
							}//check for no tileset
							if (tls.FirstGid == -1) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the position of the tile in the level

							int xx{ 0 };
							int yy{ 0 };

							//tileCounter is whichever gid we're on, or which tile we're at
							//mod width gets us the x position as, imagine arranging the tiles by row then column, with width being the max amount per row
							//then we multiply by pixels necessary
							xx = tileCounter % width;
							xx *= tileWidth;

							//this one is easier to imagine perhaps. remember tileCounter as the gid or tile we're on.
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy);

							//get the position of the tile in the tileset with this function, check code for explanation
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							//build the actual tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;

							//loops through our list of animated tiles and checks if the current tile
							//is the start of an animation. if it is, we break out with a newly changed bool and continue
							for (int i = 0; i < this->_animatedTileInfos.size(); i++) {
								if (this->_animatedTileInfos.at(i).StartTileId == gid) {
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}

							//if we got the start of an animated tile detected, put all the frames into our vector of animated tiles
							//if not, we regularly put the tile in our list of normal tiles
							if (isAnimatedTile) {
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i), tileWidth, tileHeight));
								}
								AnimatedTile tile(tilesetPositions, ati.Duration, tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
								this->_animatedTileList.push_back(tile);
							}
							
							else {
								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}

							tileCounter++;
					
							pTile = pTile->NextSiblingElement("tile");
						}
					}
					//linkedlist moving to the next data element
					pData = pData->NextSiblingElement("data");
				}
			}
			//this loops us to the next layer. think linkedlist
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	//Parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			//moving char* type from C into C++'s stringstream that w can easily work with
			ss << name;
			if (ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						//here is where we actually parse the collisions

						//add in the individual rectangles into our vector of collisionRects
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(
						std::ceil(x)*globals::SPRITE_SCALE,
						std::ceil(y)*globals::SPRITE_SCALE,
						std::ceil(width)*globals::SPRITE_SCALE,
						std::ceil(height)*globals::SPRITE_SCALE
						));
						
						//move forwards in our list of objects
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "slopes") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						//slopes can have multiple points (more than 2), like a bunch of slopes connected

						//p1 is the starting point
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));

						//further points are put into a string and then split up with our string splitter function split() and put into vectors
						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');
							//Now we have each of the pairs. Loop through the list of pairs
							//and split them into Vector2s and then store them in our points vector
							for (int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}
						//transform all the points into slopes
						for (int i = 0; i < points.size(); i += 2) {
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
			else if (ss.str() == "spawn points") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					//loop through our spawnpoints and spawn things (player, enemy, etc)
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");

						//inject the name into our ss
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;

						if (ss.str() == "player") {
							this->_spawnPoint = Vector2(std::ceil(x)*globals::SPRITE_SCALE, std::ceil(y)*globals::SPRITE_SCALE);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			else if (ss.str() == "doors") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						//make the rectangle of the door
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle(x, y, w, h);

						//this loop checks for properties... it's really just the door teleport to location property though. 
						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL) {
							while (pProperties) {
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL) {
									while (pProperty) {
										const char* name = pProperty->Attribute("name");
										std::stringstream ss;
										ss << name;

										//this is where we find the door's destination property and create the door, putting it into our list of doors
										//use the list of doors to check for collision afterwards

										if (ss.str() == "destination") {
											const char* value = pProperty->Attribute("value");
											std::stringstream ss2;
											ss2 << value;
											Door door = Door(rect, ss2.str());
											this->_doorList.push_back(door);
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
								}

								pProperties = pProperties->NextSiblingElement("properties");
							}
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			//other objectgroups go here with an else if (ss.str() == "whatever")
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}

}

void Level::update(int elapsedTime) {
	//play our animated tiles
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).update(elapsedTime);
	}
}

void Level::draw(Graphics& graphics) {

	//draw regular tiles
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}

	//draw animated tiles
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).draw(graphics);
	}





	//deprecated code, we use tiled code and xml format stuff instead
	/*
	SDL_Rect sourceRect = {0,0,64,64};
	SDL_Rect destRect;
	//the width of the level/64 equals the amount of times we need to draw it
	//same for height
	//64 is the width and height of our background tiles
	for (int x = 0; x < this->_size.x / 64; x++) {
		for (int y = 0; y < this->_size.y / 64; y++) {
			destRect.x = x * 64 * globals::SPRITE_SCALE;
			destRect.y = y * 64 * globals::SPRITE_SCALE;
			destRect.w = 64* globals::SPRITE_SCALE;
			destRect.h = 64* globals::SPRITE_SCALE;
			graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
		}
	}*/
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle& other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
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

std::vector<Door> Level::checkDoorCollisions(const Rectangle &other) {
	std::vector<Door> others;
	for (int i = 0; i < this->_doorList.size(); i++) {
		if (this->_doorList.at(i).collidesWith(other)) {
			others.push_back(this->_doorList.at(i));
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	//calculate the position of the tile in the tileset
	//just imagine it in your head or something im sure it works
	//we call null for access and format parameters since we dont care about that for some reason
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = (gid - 1) % (tilesetWidth / tileWidth);
	tsxx *= tileWidth;
	int tsyy{ 0 };
	int amt = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	return finalTilesetPosition;
}