#ifndef GRAPHICS_H
#define GRAPHICS_H

/*Holds all code relating to the graphics of the game
*/

#include <map>
#include <string>
#include <SDL.h>

struct SDL_Window;
struct SDL_Renderer;


class Graphics {
public:
	Graphics();
	~Graphics();

	/*SDL_Surface* loadImage
	Loads an image into the _spriteSheets map if it doesn't already exist
	As a result, each image will only ever be loaded once
	Returns the image from the map regardless of whether or not it was just loaded
	*/
	SDL_Surface* loadImage(const std::string &filePath);

	/*void blitSurface
	* Draws images (sdl textures) to a certain part of the screen
	* The source is what we draw, the sourceRectangle is the part of the spritesheet we are drawing, 
	and the destinationRectangle is where it's going to be drawn
	*/
	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);

	/*void flip
	* Renders everything to the screen
	*/
	void flip();

	/*void clear
	* Clears the screen
	*/
	void clear();

	/*SDL_Renderer* getRender
	* Just returns the render so we can use it in other places
	*/
	SDL_Renderer* getRender() const;


private:
	SDL_Window* _window;

	//this is what is being drawn to the screen
	//it is cleared at the start of every frame
	//blitSurface draws the screen, then flip puts that drawing into render
	SDL_Renderer* _renderer;

	std::map<std::string, SDL_Surface*> _spriteSheets;

};

#endif
