#ifndef ANIMATED_TILE_H
#define ANIMATED_TILE_H

#include "tile.h"
#include "globals.h"

#include <vector>

class AnimatedTile : public Tile {
public:
	AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 position);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
protected:
	int _amountOfTime = 0;
	int _notDone = true;
private:
	std::vector<Vector2> _tilesetPositions;
	int _tileToDraw;
	int _duration;

};

struct AnimatedTileInfo {
public:
	//FirstGid in the tileset for the animated tile
	int TilesetsFirstGid;
	//the tile at the start of the animation
	int StartTileId;
	//vector of all tiles inside of an animation
	std::vector<int> TileIds;
	int Duration;
};




#endif ANIMATED_TILE_H