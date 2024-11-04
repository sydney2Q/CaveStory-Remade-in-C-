#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <string>

#include "rectangle.h"
#include "globals.h"

 class Graphics;

/*Sprite class
* Holds on information for individual sprites
*/

class Sprite {
public:
	Sprite();

	//information on where the thing is, its graphics, the spriteSheet key, etc 
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);

	//virtual methods for derived classes as individual sprites have differing info
	virtual ~Sprite();
	virtual void update();
	//draws at location x and y
	void draw(Graphics &graphics, int x, int y);

	//returns our sprite's bounding box
	const Rectangle getBoundingBox() const;

	//returns our sprite's side that is colliding with provided other Rectangle
	const sides::Side getCollisionSide(Rectangle &other) const;

	//get the X location on the screen of a sprite
	const inline float getX() const { return this->_x; }
	//get the Y location on the screen of a sprite
	const inline float getY() const { return this->_y; }

	//setter for our X value of sourceRect for a sprite
	void setSourceRectX(int value);
	//setter for our Y value of sourceRect for a sprite
	void setSourceRectY(int value);
	//setter for our W value of sourceRect for a sprite
	void setSourceRectW(int value);
	//setter for our H value of sourceRect for a sprite
	void setSourceRectH(int value);

protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;
	float _x, _y;

	//the rectangle that represents the actual collision of the sprite
	Rectangle _boundingBox;
private:
	
};


#endif
