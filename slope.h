#ifndef SLOPE_H
#define SLOPE_H

#include "globals.h"
#include "rectangle.h"
#include<tgmath.h>

class Slope {
public:
	Slope() {}

	//constructor takes in two vector points and then finds the slope between the two vectors
	Slope(Vector2 p1, Vector2 p2) :
		_p1(p1),
		_p2(p2)
	{
		if (this->_p2.x - this->_p1.x != 0) {
			this->_slope = (fabs(this->_p2.y) - fabs(this->_p1.y)) / (fabs(this->_p2.x) - fabs(this->_p1.x));
		}
	}

	const inline float getSlope() const {
		return this->_slope;
	}

	//logic to check if a given rectangle collides with a slope
	const bool collidesWith(const Rectangle &other) const {
		return
			//this segment checks for slopes with the bottom on left, top on right
			(other.getRight() >= this->_p2.x &&
				other.getLeft() <= this->_p1.x &&
				other.getTop() <= this->_p2.y &&
				other.getBottom() >= this->_p1.y) ||
			(other.getRight() >= this->_p1.x &&
				other.getLeft() <= this->_p2.x &&
				other.getTop() <= this->_p1.y &&
				other.getBottom() >= this->_p2.y) ||

			//from here we check for slopes with the bottom on right and top on left
			(other.getLeft() <= this->_p1.x &&
				other.getRight() >= _p2.x &&
				other.getTop() <= _p1.y &&
				other.getBottom() >= _p2.y) ||
			(other.getLeft() <= this->_p2.x &&
				other.getRight() >= this->_p1.x &&
				other.getTop() <= _p2.y &&
				other.getBottom() >= _p1.y);
	}
	const inline Vector2 getP1() const { return this->_p1; }
	const inline Vector2 getP2() const { return this->_p2; }
private:
	Vector2 _p1, _p2;
	float _slope;

};

#endif