#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "animatedsprite.h"
#include "slope.h"
#include "level.h"
#include "door.h"
#include "enemy.h"

class Graphics;

class Player : public AnimatedSprite{
public:
	Player();
	Player(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	//void moveLeft
	//moves the player left by dx/by negative dx
	void moveLeft();

	//void moveRight
	//moves the player right by dx/by positive dx
	void moveRight();

	//void stopMoving
	//stops moving the player and plays the idle animation for whichever direction the player is facing
	void stopMoving();

	//void lookUp
	//called when the player wants to look upwards
	void lookUp();

	//void stopLookingUp
	//called when the player stops looking up
	void stopLookingUp();

	//void lookDown
	//called when the player wants to look down
	void lookDown();

	//void stopLookingDown
	//called when the player stops looking down
	void stopLookingDown();

	//void jump
	//starts jumping
	void jump();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();
	
	//handles our collisions. pass in the vector of rectangles that we may collide with in the level and loops through them to check if we do collide
	//handles ALL the collisions the player is colliding with
	void handleTileCollisions(std::vector<Rectangle> &others);

	//handles collisions with ALL slopes the player is colliding with
	void handleSlopeCollisions(std::vector<Slope> &others);

	//checks if we are holding the down arrow, grounded, and colliding with the door
	//if those are true we can use the door to go to another level
	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics);

	//checks if we are colliding with an enemy's box or not
	void handleEnemyCollisions(std::vector<Enemy*> &others);

	const float getX() const;
	const float getY() const;

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	//changes our current health by the amount passed in
	void adjustHealth(int amount);

private:
	float _dx, _dy;
	Direction _facing;
	bool _grounded;
	bool _lookingUp;
	bool _lookingDown;

	int _maxHealth;
	int _currentHealth;
};


#endif
