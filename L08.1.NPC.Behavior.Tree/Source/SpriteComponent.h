#ifndef SPRITE_H
#define SPRITE_H

#include"Constants.h"
#include "Component.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

class GameObject;
class GraphicsDevice;
class Texture;

class Sprite : public Component
{
public:
	Sprite(GameObject*);
	~Sprite();
	void Initialize(GraphicsDevice*, std::string);

	void Start();
	void Update();
	void Finish();

	void Draw();
	GAME_VEC position;
	float angle;
	Uint32 xPosition;
	Uint32 yPosition;

protected:
	Texture* texture;
	GraphicsDevice* gDevice;
	SDL_Rect clipArray[NUM_SPRITES];
	Uint32 stepLagCount;
	Uint32 stepCount;
	Uint32 spriteID;
	TTF_Font* font;
};


#endif