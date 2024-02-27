#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>

using namespace std;

/*

Notes:
- Add a collision system that registers every object in itself and any time an objects "Move" or "SetPosition" functions are called,
  they Update that register.
  this collision system will place its own checks for any collisions, at the end of every Update.
- Implement a Graph structure that keeps a record of all the objects locations, in order to implement path finding algorithms down
  the line... In Sha Allah.

*/

enum Direction
{
	Up,
	Right,
	Down,
	Left
};

enum BulletState
{
	Ready,
	Fired,
	Hit
};

#define NUM_OF_NPCs 5
#define NUM_OF_BULLETs 3

struct Bullet
{
	SDL_Rect rect;
	int Damage;
	int Speed;
	int State;
	bool isNPCs;
	int direction;
	int lifespan;

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
		lifespan = 200;
	}
	void Move(int direction)
	{
		switch (direction)
		{
		case Up:
			rect.x += 0;
			rect.y -= Speed;
			lifespan--;
			break;
		case Right:
			rect.x += Speed;
			rect.y += 0;
			lifespan--;
			break;
		case Down:
			rect.x += 0;
			rect.y += Speed;
			lifespan--;
			break;
		case Left:
			rect.x -= Speed;
			rect.y += 0;
			lifespan--;
			break;
		default:
			break;
		}
	}
	void Reset(int x, int y)
	{
		State = Ready;
		lifespan = 200;
		rect.x = x;
		rect.y = y;
	}
	void Draw(SDL_Renderer *renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
};

struct Entity
{
	SDL_Rect rect;
	int Health;
	int Speed;
	int Direction;
	int DamageOut;
	int DamageIn;
	bool Slowed;
	bool Moving;
	vector<Bullet> bullets;

	Entity()
	{
		rect.w = 50;
		rect.h = 50;
		rect.x = 200;
		rect.y = 200;

		Health = 100;
		Speed = 5;
		Direction = Right;
		DamageOut = 20;
		DamageIn = 20;
		Slowed = false;
		Moving = false;

		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			Bullet newBullet(rect.x, rect.y);
			bullets.push_back(newBullet);
		}
	}

	virtual void Move() = 0;
	virtual void SetPostion(int x, int y) = 0;
	virtual void Draw(SDL_Renderer *renderer, int R, int G, int B) = 0;
	virtual void Draw(SDL_Renderer *renderer) = 0;
	virtual void Shoot() = 0;
};

struct Player : public Entity
{
	Player()
	{
		rect.x = rand() % 800;
		rect.y = rand() % 600;

		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			bullets[i].isNPCs = false;
		}
	}
	void Move()
	{
		switch (Direction)
		{
		case Up:
			rect.x += 0;
			rect.y -= Speed;
			break;
		case Right:
			rect.x += Speed;
			rect.y += 0;
			break;
		case Down:
			rect.x += 0;
			rect.y += Speed;
			break;
		case Left:
			rect.x -= Speed;
			rect.y += 0;
			break;
		default:
			break;
		}
	}
	void SetPostion(int x, int y)
	{
		rect.x = x;
		rect.y = y;
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
	void Shoot()
	{
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			if (bullets[i].State == Ready)
			{
				bullets[i].State = Fired;
				bullets[i].direction = Direction;
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
	void Move()
	{
		// Move the NPC (PathFinding Algo):
	}
	void SetPostion(int x, int y)
	{
		rect.x = x;
		rect.y = y;
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
	void Shoot()
	{
	}
};

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

			Update();

			SDL_RenderPresent(renderer);
		}
	}
	void Draw()
	{
		// Draw the Player:
		player->Draw(renderer);
		// Draw the NPCs:
		for (int i = 0; i < npcLimit; i++)
		{
			NPCs[i]->Draw(renderer);
		}
		// Draw the NPC Bullets:
		for (int i = 0; i < npcLimit; i++)
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
	void Update()
	{
		// Update the Player:
		if (player->Moving)
		{
			player->Move();
			player->Moving = false;
		}

		// Update the NPCs:
		for (int i = 0; i < npcLimit; i++)
		{
			NPCs[i]->Move();
		}
		// Update the NPC Bullets:
		for (int i = 0; i < npcLimit; i++)
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