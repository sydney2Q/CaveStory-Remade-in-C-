#include "input.h"



/*Input Class:
* 
*keeps track of keyboard state
*/

//Called at the start of every new frame to reset the keys that aren't relevant
//clears all previous states of keys
void Input::beginNewFrame() {
	this->_pressedKeys.clear();
	this->_releasedKeys.clear();
}

//Called when a key is pressed
//sets pressed and held keys to true
void Input::keyDownEvent(const SDL_Event& event) {
	this->_pressedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = true;
}

//Called when a key is released
//sets released keys to true and held keys to false
void Input::keyUpEvent(const SDL_Event& event) {
	this->_releasedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = false;
}


//Self explanatory set of functions
bool Input::wasKeyPressed(SDL_Scancode key) {
	return this->_pressedKeys[key];
}
bool Input::wasKeyReleased(SDL_Scancode key) {
	return this->_releasedKeys[key];
}
bool Input::isKeyHeld(SDL_Scancode key) {
	return this->_heldKeys[key];
}