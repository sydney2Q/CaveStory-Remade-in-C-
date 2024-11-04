#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <map>
#include <string>
#include <vector>

#include "globals.h"
#include "sprite.h"

class Graphics;

/*Animated Sprite class
Holds logic for animating sprites

*/

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();

	//timeToUpdate is how long you need to go to the next frame in the animation
	AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);

	//void playAnimation
	//check if the animation we're trying to play is already playing or not
	//if it isn't, it'll start it up and reset the frames by setting _frameIndex to 0
	void playAnimation(std::string animation, bool once = false);

	//void update 
	// Updates the animated sprite with a timer
	void update(int elapsedTime);

	//void draw
	//Draws the sprites onto the screen
	void draw(Graphics &graphics, int x, int y);

protected:

	//the amount of time between frames
	double _timeToUpdate;

	//how we know the animation is only played once
	bool _currentAnimationOnce;

	//name of the current animation played
	std::string _currentAnimation;

	//void addAnimation
	//adds an animation to our map of animations for this sprite
	//works via looping through the number of frames. Each loop, it will add (i+x)*width of the x, and y
	//(i+x) simply means which frame of the spritesheet we want to be on. multiply by width to get the lenght of the x
	//Parameters: 
	// first parameter is the number of frames in the animation
	// second parameter is the x variable where the animation's frames on the spritesheet begin
	// third parameter is the y variable on the spritesheet, or you can think of it as the row the frames are on
	// fourth and fifth parameters are simply the rectangle dimensions of each frame
	// sixth parameter is the offset Vector2
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	//void resetAnimations
	//resets all animation associated with this sprite
	//simply clears out the _animations and _offsets maps
	void resetAnimations();

	//void stopAnimation
	//stops the current animation by calling animationDone on it, then resets _frameIndex to 0
	void stopAnimation();

	//void setVisible
	//sets the visibility status of the animated sprite
	void setVisible(bool visible);

	//virtual void animationDone
	//implemented here is the logic at the end of every animation, can only be implemented in inherited classes
	virtual void animationDone(std::string currentAnimation)=0;

	//virtual void setupAnimations
	//a required function that sets up all animations for a sprite, can only be implemented in inherited classes
	//MUST be implemented for all sprites
	virtual void setupAnimations()=0;
private:
	//KEY is animation name. VALUE is the individual rectangles (frames on the spritesheet) of each animation
	std::map<std::string, std::vector<SDL_Rect>> _animations;

	
	std::map<std::string, Vector2> _offsets;

	//which frame we're at currently
	int _frameIndex;

	//for the timer
	double _timeElapsed = 0;

	//whether or not the animation is visible
	bool _visible;



};



#endif
