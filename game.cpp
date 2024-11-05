#include <SDL.h>

#include "game.h"
#include "graphics.h"
#include "input.h"
#include "slope.h"

/*Game class:
* holds ALL information for the MAIN game loop. 
* this is where everything is happening. 
*/

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;

}

//the constructor
Game::Game() {
	//initializes a number of other things like events, audio, timer, etc
	SDL_Init(SDL_INIT_EVERYTHING);
	//calls the gameLoop function: immediately after starting, the game begins
	this->gameLoop();
}

Game::~Game() {

}

void Game::gameLoop() {
	//creates a game window by creating a Graphics class
	//Graphics graphics;
	//events in this frame: if something happens, it is stored in the event var
	SDL_Event event;

	this->_level = Level("Map 1", _graphics);
	this->_player = Player(_graphics, this->_level.getPlayerSpawnPoint());
	this->_hud = HUD(_graphics, this->_player);

	Input input;

	//gets the number of milliseconds after the SDL library was initialized
	int LAST_UPDATE_TIME = SDL_GetTicks();


	//game loop start
	while (true) {
		input.beginNewFrame();

		//we pass a pointer to the event we made: if SDL detects an event, it is sent to the event var
		if (SDL_PollEvent(&event)) {
			//checks for a keypress
			if (event.type == SDL_KEYDOWN) {
				//checks for if this key is NOT being HELD down
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			//checks for key release
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			//check for if we want to end the program
			else if (event.type == SDL_QUIT) {
				return;
			}
			
		}
		if (input.isKeyHeld(SDL_SCANCODE_LEFT)) {
			this->_player.moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_player.moveRight();
		}

		if (input.isKeyHeld(SDL_SCANCODE_UP)) {
			this->_player.lookUp();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN)) {
			this->_player.lookDown();
		}

		if (input.wasKeyReleased(SDL_SCANCODE_UP)) {
			this->_player.stopLookingUp();
		}
		if (input.wasKeyReleased(SDL_SCANCODE_DOWN)) {
			this->_player.stopLookingDown();
		}

		if (input.wasKeyPressed(SDL_SCANCODE_Z) && input.isKeyHeld(SDL_SCANCODE_Z)) {
			_player.jump();
		}
		
		if (!input.isKeyHeld(SDL_SCANCODE_RIGHT) && !input.isKeyHeld(SDL_SCANCODE_LEFT)) {
			this->_player.stopMoving();
		}

		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
			return;
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();

		//update our graphics object in case we went through a door or something
		//this->_graphics = graphics;

		//how long the last frame took 
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		//physics are based on the frames, so it's important to have the frames be at minimum the max frame time
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));

		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(_graphics);
	}
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	this->_level.draw(graphics);
	this->_player.draw(graphics);
	this->_hud.draw(graphics);

	graphics.flip();

}

void Game::update(float elapsedTime) {
	this->_player.update(elapsedTime);
	this->_level.update(elapsedTime, this->_player);
	this->_hud.update(elapsedTime, this->_player);

	//check {
	//collision
	std::vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
		//player collided with at least one tile
		this->_player.handleTileCollisions(others);
	}
	//check slope collision
	std::vector<Slope> otherSlopes;
	if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleSlopeCollisions(otherSlopes);
	}
	//check door collision
	std::vector<Door> otherDoors;
	if ((otherDoors = this->_level.checkDoorCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleDoorCollision(otherDoors, this->_level, this->_graphics);
	}

	//check enemy collision
	std::vector<Enemy*> otherEnemies;
	if ((otherEnemies = this->_level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleEnemyCollisions(otherEnemies);
	}

}