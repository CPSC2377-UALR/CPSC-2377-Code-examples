/*
* Author:	Keith Bush
*			UALR
*
* Date:		March 11, 2015
*
* File:		Source.cpp
*
* Purpose:	Demonstrate the use of contact listener
*/

//System Headers
#include <iostream>

//Library Headers
#include "SDL.h"
#include "Box2D.h"

//Game Headers
#include "GraphicsDevice.h"
#include "Timer.h"
#include "GameObject.h"
#include "ContactListener.h"
#include "Constants.h"


int main(int argc, char *argv[])
{

	//Screen dimensions
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 600;

	//Time per frame
	float dt = 1.0f / 50.0f;

	//Construct Graphical Device
	GraphicsDevice* gDevice = new GraphicsDevice(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!gDevice->Initialize(true))
	{
		printf("Graphics Device could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	//Construct Event
	SDL_Event* event = new SDL_Event();
	if (!event) {
		printf("SDL Event could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	//Construct Frame Timer
	Timer* fps = new Timer();
	if (!fps->Initialize((Uint32)(1 / dt))) {
		printf("Frame Timer could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	//
	// TODO 01 Construct Physics World
	//
	const b2Vec2 gravity(RW2PW(0), RW2PW(+200));
	b2World* world = new b2World(gravity);

	//Set boundaries of world (Render->Physics)
	const b2Vec2 vTopLeft = b2Vec2(RW2PW(0), RW2PW(0));
	const b2Vec2 vTopRight = b2Vec2(RW2PW(SCREEN_WIDTH), RW2PW(0));
	const b2Vec2 vBottomLeft = b2Vec2(RW2PW(0), RW2PW(SCREEN_HEIGHT));
	const b2Vec2 vBottomRight = b2Vec2(RW2PW(SCREEN_WIDTH), RW2PW(SCREEN_HEIGHT));

	//
	//Create the world boundaries
	//
	b2BodyDef bd;
	b2Body* edge = world->CreateBody(&bd);
	b2EdgeShape shape;

	//Set-up debuggin
	world->SetDebugDraw(NULL);

	//Create bottom boundary
	shape.SetTwoSided(vBottomLeft, vBottomRight);
	edge->CreateFixture(&shape, 0);

	//Create left boundary
	shape.SetTwoSided(vBottomLeft, vTopLeft);
	edge->CreateFixture(&shape, 0);

	//Create right boundary
	shape.SetTwoSided(vBottomRight, vTopRight);
	edge->CreateFixture(&shape, 0);

	//
	// TODO 02 Initialize Game Objects
	//
	GameObject* object1 = new GameObject();
	object1->Initialize(gDevice->getRenderer(), "./Assets/Images/Ball.png", world, 300, 50);

	GameObject* object2 = new GameObject();
	object2->Initialize(gDevice->getRenderer(), "./Assets/Images/BallBearing.png", world, 300, 250);

	//
	// TODO 03 Define Contact Listener
	//
	ContactListener* cl = new ContactListener();

	//Add the contact listener the physics world
	world->SetContactListener(cl);

	//Initialize Termination Criteria
	bool quit = false;

	//While the user hasn't quit
	while (!quit)
	{

		//Start the frame's time
		fps->start();

		//Check for an event
		if (SDL_PollEvent(event))
		{
			//Translate Event to QUIT signal
			if (event->type == SDL_QUIT)
			{
				quit = true;
			}
		}

		//
		//Object handles relevant events
		//
		object1->HandleEvent(event);
		object2->HandleEvent(event);

		//
		//Conduct the Game Logic update (i.e., physics/animation)
		//
		object1->Update(dt);
		object2->Update(dt);

		//
		// Conduct Physics update. Trigger the solver each iteration of the game loop
		//
		world->Step(dt, 8, 3);

		//
		//Game state updates complete, let's draw
		//
		gDevice->Begin();

		//Draw a game object
		object1->Draw();
		object2->Draw();

		//End the frame and draw to window
		gDevice->Present();

		//Buffer the Frames per second (if necessary)
		fps->fpsRegulate();

		//SDL_Delay(4000);// DEBUG

	}

	return 0;

}