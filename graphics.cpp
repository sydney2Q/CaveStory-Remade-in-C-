#include <SDL.h>
#include "graphics.h"
#include <SDL_image.h>
#include "globals.h"
/*
Graphics Class:

holds all information dealing with graphics in the game


*/


/*Graphics constructor implementation:
* 
*/
Graphics::Graphics() {
	/*creates the game window
		*
		* paramaters:width, height, and flags(we add zero for no flags in this case)
		* and then a pointer for window and renderer
		* 
	*/
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);

	
	SDL_SetWindowTitle(this->_window, "Cavestory");
}

/*Destructor implementation:
* pass the window to be destroyed
*/

Graphics::~Graphics() {
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string& filePath) {

	//zero means that it hasn't been loaded yet
	if (this->_spriteSheets.count(filePath)==0) {

		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return this->_spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip() {
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRender() const {
	return this->_renderer;
}

