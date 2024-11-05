#include "sprite.h"
#include "graphics.h"
#include <SDL.h>
#include "globals.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY) :
	_x(posX),
	_y(posY)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.h = height;
	this->_sourceRect.w = width;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRender(), graphics.loadImage(filePath));
	if (this->_spriteSheet == NULL) {
		printf("\nError:Unable to load image\n");
	}

	this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

Sprite::~Sprite(){}

void Sprite::draw(Graphics &graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w *globals::SPRITE_SCALE, 
		this->_sourceRect.h*globals::SPRITE_SCALE};
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::update() {
	this->_boundingBox = Rectangle(this->_x, this->_y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE);
}

const Rectangle Sprite::getBoundingBox() const{
	return this->_boundingBox;
}


const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	//we can get the side we are colliding with by finding the SMALLEST dimensional overlap between the two rectangles
	//the amount that each rectangle is overlapping with the other
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = this->getBoundingBox().getRight() - other.getLeft();
	amtLeft = other.getRight() - this->getBoundingBox().getLeft();
	amtTop = other.getBottom() - this->getBoundingBox().getTop();
	amtBottom = this->getBoundingBox().getBottom() - other.getTop();

	//get the smallest overlap
	int lowest;
	lowest = std::min({ amtRight, amtLeft, amtTop, amtBottom });

	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}

void Sprite::setSourceRectX(int value) {
	this->_sourceRect.x = value;
}

void Sprite::setSourceRectY(int value) {
	this->_sourceRect.y = value;
}

void Sprite::setSourceRectW(int value) {
	this->_sourceRect.w = value;
}

void Sprite::setSourceRectH(int value) {
	this->_sourceRect.h = value;
}