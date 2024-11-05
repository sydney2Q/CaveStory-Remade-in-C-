#include "player.h"
#include "graphics.h"

Player::Player() {}

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Player::Player(Graphics& graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "Content/sprites/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false),
	_lookingUp(false),
	_lookingDown(false),
	_maxHealth(3),
	_currentHealth(3)
{
	graphics.loadImage("Content/sprites/MyChar.png");
	this->setupAnimations();
	this->playAnimation("RunRight");
}

void Player::setupAnimations() {
	//idle animation when facing left
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));

	//idle animation when facing right
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));

	//animation that runs left
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));

	//animation for running rightwards
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));

	//animation for standing still and looking up, facing left
	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0,0));

	//animation for standing still and looking up, facing right
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0,0));

	//animation for running left while looking up
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0));

	//animation for running right while looking up
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0,0));

	//animation for looking down while facing left in the air
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0));

	//animation for looking down while facing right in the air
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0));

	//animation for looking backwards, or interacting with objects while facing left
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));

	//animation for looking backwards, or interacting with objects while facing right
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation){}

const float Player::getX() const {
	return this->_x;
}
const float Player::getY() const {
	return this->_y;
}

void Player::moveLeft() {
	//if we're looking down we don't want quote to move
	if (this->_lookingDown && _grounded) {
		return;
	}
	//if not looking up run the usual left run
	this->_dx = -player_constants::WALK_SPEED;
	if (this->_lookingUp != true) {
		this->playAnimation("RunLeft");
	}
	this->_facing = LEFT;
}

void Player::moveRight() {
	if (this->_lookingDown && _grounded) {
		return;
	}

	this->_dx = player_constants::WALK_SPEED;

	if (this->_lookingUp!=true) {
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	if (this->_lookingUp != true && this->_lookingDown != true) {
		this->playAnimation(this->_facing == LEFT ? "IdleLeft" : "IdleRight");
	}
}

void Player::lookUp() {
	this->_lookingUp = true;
	if (this->_dx == 0) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp() {
	this->_lookingUp = false;
}

void Player::lookDown() {
	this->_lookingDown = true;
	if (this->_grounded == true) {
		this->playAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "LookDownRight" : "LookDownLeft");
	}
}

void Player::stopLookingDown() {
	this->_lookingDown = false;
}

void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0;
		_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	//first, figure out what side the collision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0;
				this->_y = others.at(i).getBottom() + 1;
				//this is for colliding with a tile above you when running up a slope
				if (this->_grounded) {
					this->_dx = 0;
					this->_x -= this->_facing == RIGHT ? 0.1f : -0.1f;
				}
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
			}
		}
	}
}

void Player::handleSlopeCollisions(std::vector<Slope> &others) {
	for (int i = 0; i < others.size(); i++) {
		//first, calculate where on the slope the player's bottom center is touching
		//then use y = mx+b to figure out the y-position to place him at.
		//first calculate b, the slope intercept using one of the points
		//b = y-mx, we use the two points on our slope as x and y
		int b = others.at(i).getP1().y-(others.at(i).getSlope()*fabs(others.at(i).getP1().x));

		//get the center of player
		int centerX = this->_boundingBox.getCenterX();
		//pass this X into the y=mx+b equation to get the new y-position

		int newY = (others.at(i).getSlope() * centerX + b - 8); //8 is a temporary fix for now due to some rounding error or something
		
		//reposition the player to the correct y-posiion
	
		if (this->_grounded) {
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true;
		}

	}
}

void Player::handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics) {
	for (int i = 0; i < others.size(); i++) {
		if (this->_grounded && this->_lookingDown) {
			level = Level(others.at(i).getDestination(), graphics);
			this->_x = level.getPlayerSpawnPoint().x;
			this->_y = level.getPlayerSpawnPoint().y;
		}
	}
}

void Player::handleEnemyCollisions(std::vector<Enemy*> &others) {
	for (int i = 0; i < others.size(); i++) {
		others.at(i)->touchPlayer(this);
	}
}

void Player::adjustHealth(int amount) {
	this->_currentHealth += amount;
}

void Player::update(float elapsedTime) {
	//apply gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}

	//move by dx and dy
	this->_x += this->_dx * elapsedTime;
	this->_y += this->_dy * elapsedTime;
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}