#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "SDL.h"
#include "SDL_image.h"

class Texture {

public:
	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool load(SDL_Renderer* renderer, std::string path);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = nullptr);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:

	//The actual hardware texture
	SDL_Texture* texture{ nullptr };

	//Image dimensions
	int width{ 0 };
	int height{ 0 };

};

#endif