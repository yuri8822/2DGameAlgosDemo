#include <SDL.h>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>

using namespace std;

enum Color { Ready, Fired, Hit};


struct Bullet
{
	int Damage = 10;
	int Speed;
	int State = Ready;
};

struct Entity
{
	SDL_Rect rect;
	int Health;
	int DamageOut;
	int DamageIn;
	bool Slowed;


	Entity()
	{
		rect.w = 50;
		rect.h = 50;
		rect.x = 200;
		rect.y = 200;

		Health = 100;
		DamageOut = 20;
		DamageIn = 20;
		Slowed = false;
	}

	virtual void Move(int x, int y) = 0;
	virtual void Draw(SDL_Renderer *renderer, int R, int G, int B) = 0;
	virtual void Draw(SDL_Renderer *renderer) = 0;
};

struct Player : public Entity
{
	Player()
	{
		rect.x = rand() % 800;
		rect.y = rand() % 600;
	}
	void Move(int x, int y)
	{
		rect.x += x;
		rect.y += y;
	}
	void Draw(SDL_Renderer *renderer, int R, int G, int B)
	{
		SDL_SetRenderDrawColor(renderer, R, G, B, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
	void Draw(SDL_Renderer *renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
};

struct NPC : public Entity
{
	NPC()
	{
		rect.x = rand() % 800;
		rect.y = rand() % 600;
	}
	void Move(int x, int y)
	{
	}
	void Draw(SDL_Renderer *renderer, int R, int G, int B)
	{
		SDL_SetRenderDrawColor(renderer, R, G, B, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
	void Draw(SDL_Renderer *renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
};

struct EntityManager
{
	vector<Entity*> NPCs;

};

int main(int argc, char *argv[])
{
	// Initializers:
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(NULL));
	EntityManager EntityManager;
	Entity *player = new Player;
	int npcLimit = rand() % 5 + 1;
	for (int i = 0; i < npcLimit; i++)
	{
		Entity *npc = new NPC;
		EntityManager.NPCs.push_back(npc);
	}


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
	// Create a window																									   //
	SDL_Window *window = SDL_CreateWindow("2D Game Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);    //
	// Create a renderer																								   //
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);		   //
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

	//////

	// Game loop
	bool running = true;
	SDL_Event event;
	while (running)
	{
		// Process events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:			 // Left arrow key
					player->Move(-5, 0); // Move player to the left
					break;
				case SDLK_RIGHT:		 // Right arrow key
					player->Move(5, 0); // Move player to the right
					break;
				case SDLK_UP:			// Up arrow key
					player->Move(0, -5); // Move player up
					break;
				case SDLK_DOWN:			// Down arrow key
					player->Move(0, 5); // Move player down
					break;
				}
			}
		}

		// Clear the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw the game world:

		// Draw The Player:
		player->Draw(renderer);

		// Draw The NPC:
		for (int i = 0; i < npcLimit; i++)
		{
			EntityManager.NPCs[i]->Draw(renderer);
		}

		// Show the frame
		SDL_RenderPresent(renderer);
	}

	return 0;
}