#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>

#include "Entity.h"
#include "Bullet.h"
#include "CollisionSystem.h"

using namespace std;

#define NUM_OF_NPCs 5
#define NUM_OF_BULLETs 4

class Engine
{
private:
	vector<Entity *> NPCs;
	Entity *player;
	int npcLimit;
	SDL_Window *window;
	SDL_Renderer *renderer;

public:
	Engine()
	{
		SDL_Init(SDL_INIT_VIDEO);

		player = new Player;
		npcLimit = rand() % NUM_OF_NPCs + 1;
		for (int i = 0; i < npcLimit; i++)
		{
			Entity *npc = new NPC;
			NPCs.push_back(npc);
		}
		window = SDL_CreateWindow("2D Game Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}
	void GameLoop()
	{
		bool running = true;
		while (running)
		{
			// Clear the screen
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			Draw();

			running = EventListener();

			CheckCollision();

			Update();

			SDL_RenderPresent(renderer);
		}
	}
	void Draw()
	{
		// Draw the Player:
		player->Draw(renderer);
		// Draw the NPCs:
		for (int i = 0; i < NPCs.size(); i++)
		{
			NPCs[i]->Draw(renderer);
		}
		// Draw the NPC Bullets:
		for (int i = 0; i < NPCs.size(); i++)
		{
			for (int j = 0; j < NUM_OF_BULLETs; j++)
			{
				NPCs[i]->bullets[j].Draw(renderer);
			}
		}
		// Draw the Player Bullets:
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			player->bullets[i].Draw(renderer);
		}
	}
	bool EventListener()
	{
		bool running = true;
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return false;
					break;
				case SDLK_UP:
					player->Direction = Up;
					player->Moving = true;
					break;
				case SDLK_RIGHT:
					player->Direction = Right;
					player->Moving = true;
					break;
				case SDLK_DOWN:
					player->Direction = Down;
					player->Moving = true;
					break;
				case SDLK_LEFT:
					player->Direction = Left;
					player->Moving = true;
					break;
				case SDLK_SPACE:
					player->Shoot();
					break;
				default:
					break;
				}
			}
		}
		return running;
	}
	void CheckCollision()
	{
		// check for collisions:

		// Check for Player Bullets hitting the NPCs:
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			for (int j = 0; j < NPCs.size(); j++)
			{
				if (player->bullets[i].frame == NPCs[j]->frame)
				{
					NPCs[j]->Health -= player->bullets[i].Damage * player->DamageOut;
					player->bullets[i].Reset(player->rect.x, player->rect.y);
				}
			}
		}
	}
	void Update()
	{
		// Update the Player:
		if (player->Moving)
		{
			player->Move();
			player->Moving = false;
		}

		// Update the NPCs:
		for (int i = 0; i < NPCs.size(); i++)
		{
			if (NPCs[i]->Health <= 0)
			{
				NPCs.erase(NPCs.begin() + i);
			}
		}
		for (int i = 0; i < NPCs.size(); i++)
		{
			NPCs[i]->Move();
		}
		// Update the NPC Bullets:
		for (int i = 0; i < NPCs.size(); i++)
		{
			for (int j = 0; j < NUM_OF_BULLETs; j++)
			{
				NPCs[i]->bullets[j].Reset(NPCs[i]->rect.x, NPCs[i]->rect.y);
			}
		}
		// Update the Player Bullets (also check for when a certain bullet is fired):
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			if (player->bullets[i].State == Fired)
			{
				if (player->bullets[i].lifespan > 0)
				{
					player->bullets[i].Move(player->bullets[i].direction);
				}
				else
				{
					player->bullets[i].Reset(player->rect.x, player->rect.y);
				}
			}
			else if (player->bullets[i].State == Ready)
			{
				player->bullets[i].Reset(player->rect.x, player->rect.y);
			}
		}
	}
	~Engine()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
};

int main(int argc, char *argv[])
{
	srand(time(NULL));

	Engine engine;
	engine.GameLoop();
	return 0;
}