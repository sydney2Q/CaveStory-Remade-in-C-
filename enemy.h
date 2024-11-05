#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"
#include "globals.h"
#include "player.h"

#include <string>

class Graphics;

class Enemy : public AnimatedSprite {
public:
	Enemy();
	Enemy(Graphics& graphics, std::string filePath, int sourceX, int sourceY,
		int width, int height, Vector2 spawnPoint, int timeToUpdate);
	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);

	virtual void touchPlayer(Player* player) = 0;

	//getter for maxHealth of an enemy
	const inline int getMaxHealth() const { return this->_maxHealth; }
	//getter for the currentHealth of an enemy
	const inline int getCurrentHealth() const { return this->_currentHealth; }
protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
};

#endif ENEMY_H