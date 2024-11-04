#ifndef TILE_H
#define TILE_H

#include "globals.h"
struct SDL_Texture;
class Graphics;


class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);
	//for collisions and etcetera
	void update(int elapsedTime);
	void draw(Graphics &graphics);
protected:
	//Each tile comes from a tileset, and this keeps track of where it came from
	SDL_Texture* _tileset;
	//size of tile
	Vector2 _size;
	//position on the tileset the tile is
	Vector2 _tilesetPosition;
	//position in the map the tile is
	Vector2 _position;

};


#endif TILE_H