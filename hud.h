#ifndef HUD_H
#define HUD_H

#include "sprite.h"
#include "player.h"

class Graphics;

class HUD {

public:
	HUD();
	HUD(Graphics &g, Player &player);
	void update(int elapsedTime);
	void draw(Graphics &graphics);

private:
	Player _player;


	/// HEALTH SPRITES ///
	//sprite for our healthbar, the darker underlaid one for missing hp
	Sprite _healthBarSprite;

	//sprite for our health number in the ones digits
	Sprite _healthNumber1;
	
	//sprite for our current health, lighter red that corresponds with the percent health remaning
	Sprite _currentHealthBar;


	/// EXP SPRITES ///
	//sprite for the HUD that just says Lv for our level
	Sprite _lvWord;

	//sprite for the level of our gun
	Sprite _lvNumber;

	//sprite for the exp bar of our gun, the darker underlay for missing exp
	Sprite _expBar;


	//// WEAPONS INFO ///
	//sprite that is just a seperator for text, nothing important
	Sprite _slash;

	//sprite that is a placeholder for numbers related to guns, for when you don't have a gun/for that gun the number is not applicable
	Sprite _dashes;

};


#endif HUD_H
