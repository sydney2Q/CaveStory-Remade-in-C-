#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <map>

class Input {
public:
	//called at the start of every frame
	void beginNewFrame();
	//called when key is released
	void keyUpEvent(const SDL_Event& event);
	//called when key is pressed
	void keyDownEvent(const SDL_Event& event);

	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);

private:
	//map is basically a hashmap. Scancode holds key presses
	//So, the first component stores all possible keys and the second
	//stores if they are true or false
	std::map<SDL_Scancode, bool> _heldKeys;
	std::map<SDL_Scancode, bool> _pressedKeys;
	std::map<SDL_Scancode, bool> _releasedKeys;

};

#endif
