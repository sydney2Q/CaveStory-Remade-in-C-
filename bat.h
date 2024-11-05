#ifndef BAT_H
#define BAT_H

#include "enemy.h"


class Bat : public Enemy {
public:
	Bat();
	//constructor takes in graphics and then a spawnpoint for the bat
	Bat(Graphics &graphics, Vector2 spawnPoint);
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	//touching the player makes you lose ONE health
	void touchPlayer(Player* player);

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp;
	
};

#endif BAT_H