#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>

using namespace std;

enum BulletState
{
	Ready,
	Fired,
	Hit
};

#define NUM_OF_NPCs 10
#define NUM_OF_BULLETs 6

struct Bullet
{
	SDL_Rect rect;
	int Damage;
	int Speed;
	int State;
	bool isNPCs;

	Bullet()
	{
	}
	Bullet(int x, int y)
	{
		rect.w = 5;
		rect.h = 5;
		rect.x = x;
		rect.y = y;

		Damage = 10;
		Speed = 5;
		State = Ready;
	}
	void SetPosition(SDL_Renderer *renderer, int x, int y)
	{
		rect.x += x;
		rect.y += y;
	}
	void Draw(SDL_Renderer *renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
	void Shoot(int direction)
	{
		switch (direction)
		{
		case 0:

			break;
		default:
			break;
		}
	}
};

struct Entity
{
	SDL_Rect rect;
	int Health;
	int DamageOut;
	int DamageIn;
	bool Slowed;
	vector<Bullet> bullets;

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

		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			Bullet newBullet(rect.x, rect.y);
			bullets.push_back(newBullet);
		}
	}

	virtual void Move(SDL_Renderer *renderer, int x, int y) = 0;
	virtual void Draw(SDL_Renderer *renderer, int R, int G, int B) = 0;
	virtual void Draw(SDL_Renderer *renderer) = 0;
	virtual void Shoot(int direction) = 0;
};

struct Player : public Entity
{
	Player()
	{
		rect.x = rand() % 800;
		rect.y = rand() % 600;
	}
	void Move(SDL_Renderer *renderer, int x, int y)
	{
		// Move the Player:
		rect.x += x;
		rect.y += y;

		// Move bullet to players new location:
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			bullets[i].SetPosition(renderer, rect.x, rect.y);
		}
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
	void Shoot(int direction)
	{
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			if (bullets[i].State == Ready)
			{
				bullets[i].State = Fired;
				bullets[i].Shoot(direction);
				break;
			}
		}
	}
};

struct NPC : public Entity
{
	NPC()
	{
		rect.x = rand() % 800;
		rect.y = rand() % 600;
	}
	void Move(SDL_Renderer *renderer, int x, int y)
	{
		// Move the NPC (PathFinding Algo):

		// Move bullet to NPCs new location:
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			bullets[i].SetPosition(renderer, rect.x, rect.y);
		}
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
	void Shoot(int direction)
	{
	}
};

int main(int argc, char *argv[])
{
	// Initializers:
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(NULL));
	vector<Entity *> NPCs;
	Entity *player = new Player;
	int direction = 0;
	int npcLimit = rand() % NUM_OF_NPCs + 1;
	for (int i = 0; i < npcLimit; i++)
	{
		Entity *npc = new NPC;
		NPCs.push_back(npc);
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
	// Create a window																									   //
	SDL_Window *window = SDL_CreateWindow("2D Game Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0); //
	// Create a renderer																								   //
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

	//////

	// Game loop
	bool running = true;
	SDL_Event event;
	while (running)
	{
		// Clear the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw The Player:
		player->Draw(renderer);

		// Draw The NPC:
		for (int i = 0; i < npcLimit; i++)
		{
			NPCs[i]->Draw(renderer);
		}

		// Process events (I need to make this multithreaded)
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
				case SDLK_LEFT:					   // Left arrow key
					player->Move(renderer, -5, 0); // Move player to the left
					direction = 3;
					break;
				case SDLK_RIGHT:				  // Right arrow key
					player->Move(renderer, 5, 0); // Move player to the right
					direction = 1;
					break;
				case SDLK_UP:					   // Up arrow key
					player->Move(renderer, 0, -5); // Move player up
					direction = 0;
					break;
				case SDLK_DOWN:					  // Down arrow key
					player->Move(renderer, 0, 5); // Move player down
					direction = 2;
					break;
				case SDLK_f:
					player->Shoot(direction);
					break;
				}
			}
		}

		// Draw Everything:

		// Draw the Player:
		player->Draw(renderer);
		// Draw the NPCs:
		for (int i = 0; i < npcLimit; i++)
		{
			NPCs[i]->Draw(renderer);
		}
		// Draw the Bullets:
		for (int i = 0; i < npcLimit; i++)
		{
			for (int j = 0; i < NUM_OF_BULLETs; i++)
			{
				NPCs[i]->bullets[j].Draw(renderer);
			}
		}

			// Show the frame
			SDL_RenderPresent(renderer);
	}

	return 0;
}