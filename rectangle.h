#ifndef RECTANGLES_H
#define RECTANGLES_H

#include "globals.h"

class Rectangle {
public:
	Rectangle(){}

	Rectangle(int x, int y, int width, int height):
		_x(x),
		_y(y),
		_width(width),
		_height(height)

	{}

	const int getCenterX() const {return this->_x + this->_width / 2;}
	const int getCenterY() const {return this->_y + this->_height / 2;}

	const int getLeft() const {return this->_x;}
	const int getRight() const {return this->_x + this->_width;}
	const int getTop() const {return this->_y;}
	const int getBottom() const {return this->_y + this->_height;}

	const int getWidth() const {return this->_width;}
	const int getHeight() const {return this->_height;}

	//returns the respective side of rectangle using above functions
	const int getSide(const sides::Side side) const {
		return
			side == sides::LEFT ? this->getLeft() :
			side == sides::RIGHT ? this->getRight() :
			side == sides::TOP ? this->getTop() :
			side == sides::BOTTOM ? this->getBottom() :
			sides::NONE;

	}

	//function takes in another rectangle and checks if they are colliding or not
	const bool collidesWith(const Rectangle &other) const {
		return
			this->getRight() >= other.getLeft() &&
			this->getLeft() <= other.getRight() &&
			this->getTop() <= other.getBottom() &&
			this->getBottom() >= other.getTop();
	}

	bool collidesWithNOTCONST( Rectangle& other) {
		return
			this->getRight() >= other.getLeft() &&
			this->getLeft() <= other.getRight() &&
			this->getTop() <= other.getBottom() &&
			this->getBottom() >= other.getTop();
	}

	const bool isValidRectangle() const {
		return (this->_x>=0&&this->_y>=0&&this->_width>=0&&this->_height>=0);
	}

	//return a the rectangle for this rectangle, dereferenced from a pointer
	const inline Rectangle getRect() const { return *this; }


private:
	//_x and _y are the location of the top right corner of the rectangle
	//height and width are the top to down and right to left length extending from x y respectively
	int _x, _y, _width, _height;

};


#endif RECTANGLES_H