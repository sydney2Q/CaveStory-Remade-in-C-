#include "bat.h"

Bat::Bat() {}

Bat::Bat(Graphics& graphics, Vector2 spawnPoint) :
	Enemy(graphics, "Content/sprites/NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140), 
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false)
{
	this->setupAnimations();
	this->playAnimation("FlyLeft");
}

void Bat::update(int elapsedTime, Player &player) {
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");

	//logic to move up or down
	this->_y += this->_shouldMoveUp ? -0.05 : 0.05;
	if (this->_y > (this->_startingY + 40) || this->_y < this->_startingY - 40) {
		this->_shouldMoveUp = !this->_shouldMoveUp;
	}

	Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Bat::animationDone(std::string currentAnimation){}

void Bat::setupAnimations() {
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
}

void Bat::touchPlayer(Player* player) {
	player->adjustHealth(-1);
}