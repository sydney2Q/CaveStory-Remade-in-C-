#ifndef LEVEL_H
#define LEVEL_H

#include "rectangle.h"
#include "graphics.h"
#include "globals.h"
#include "tile.h"
#include "slope.h"
#include "animatedtile.h"
#include "door.h"

#include <vector>

struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Graphics;


class Level {
public:
	Level();
	Level(std::string mapName, Graphics &graphics);
	~Level();
	void update(int elapsedTime);
	//this thing actually loops to draw the image multiple times to cover the screen
	void draw(Graphics &graphics);

	//goes through all the collisionRects in this level and checks if they are colliding with the provided rectangle
	//returns the list of all rectangles the current rectangle passed is coliding with
	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);

	//checks for slope collisions similarly to rectangle collisions
	std::vector<Slope> checkSlopeCollisions(const Rectangle &other);

	//check for door collisions 
	std::vector<Door> checkDoorCollisions(const Rectangle &other);

	const Vector2 getPlayerSpawnPoint() const;

private:
	std::string _mapName;
	Vector2 _spawnPoint;

	Vector2 _size;
	Vector2 _tileSize;

	SDL_Texture* _backgroundTexture;
	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;
	std::vector<Slope> _slopes;

	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedTileInfos;

	std::vector<Door> _doorList;

	//void loadMap
	//is private because we only ever call it inside the class
	//loads a map
	void loadMap(std::string mapName, Graphics& graphics);

	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
};

//Tileset structure
struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;

	Tileset() {
		this->FirstGid = -1;
	}
	Tileset(SDL_Texture* texture, int firstGid) {
		this->Texture = texture;
		this->FirstGid = firstGid;

	}
};

#endif 