#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Asteroid.h"

int main(int argc, char *argv[]){
	Asteroid game;
	while (!game.UpdateAndRender()) {}
	return 0;
}