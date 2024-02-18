#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>

using namespace std;

/*

Notes:
- Make the Bullet struct independant of the Entity struct.

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
	void Move(int x, int y)
	{
		rect.x += x;
		rect.y += y;
	}
	void SetPosition(int x, int y)
	{
		rect.x = x;
		rect.y = y;
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
		case Up:
			Move(0, -Speed);
			break;
		case Right:
			Move(Speed, 0);
			break;
		case Down:
			Move(0, Speed);
			break;
		case Left:
			Move(-Speed, 0);
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
	bool Moving;
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
		Moving = false;

		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			Bullet newBullet(rect.x, rect.y);
			bullets.push_back(newBullet);
		}
	}

	virtual void Move(int x, int y) = 0;
	virtual void SetPostion(int x, int y) = 0;
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
	void Move(int x, int y)
	{
		// Move the Player:
		rect.x += x;
		rect.y += y;

		// Move bullet to players new location:
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			bullets[i].Move(rect.x, rect.y);
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
	void Move(int x, int y)
	{
		// Move the NPC (PathFinding Algo):

		// Move bullet to NPCs new location:
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			bullets[i].Move(rect.x, rect.y);
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
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
	void Shoot(int direction)
	{
	}
};

class Engine
{
private:
	vector<Entity *> NPCs;
	Entity *player;
	int direction;
	int npcLimit;
	SDL_Window *window;
	SDL_Renderer *renderer;
	vector <Bullet> firedBullets;

public:
	Engine()
	{
		SDL_Init(SDL_INIT_VIDEO);

		player = new Player;
		direction = Up;
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
					direction = Up;
					player->Moving = true;
					break;
				case SDLK_RIGHT:
					direction = Right;
					player->Moving = true;
					break;
				case SDLK_DOWN:
					direction = Down;
					player->Moving = true;
					break;
				case SDLK_LEFT:
					direction = Left;
					player->Moving = true;
					break;
				case SDLK_SPACE:
					player->Shoot(direction);
					player->bullets[0].State = Fired;
					firedBullets.push_back(player->bullets[0]); //// this is where i left off
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
		switch (direction)
		{
		case Up:
			if (player->Moving)
			{
				player->Move(0, -5);
				player->Moving = false;
			}
			break;
		case Right:
			if (player->Moving)
			{
				player->Move(5, 0);
				player->Moving = false;
			}
			break;
		case Down:
			if (player->Moving)
			{
				player->Move(0, 5);
				player->Moving = false;
			}
			break;
		case Left:
			if (player->Moving)
			{
				player->Move(-5, 0);
				player->Moving = false;
			}
			break;
		default:
			break;
		}
		// Update the NPCs:
		for (int i = 0; i < npcLimit; i++)
		{
			NPCs[i]->Move(0, 0);
		}
		// Update the NPC Bullets:
		for (int i = 0; i < npcLimit; i++)
		{
			for (int j = 0; j < NUM_OF_BULLETs; j++)
			{
				NPCs[i]->bullets[j].SetPosition(NPCs[i]->rect.x, NPCs[i]->rect.y);
			}
		}
		// Update the Player Bullets (also check for when a certain bullet is fired):
		for (int i = 0; i < NUM_OF_BULLETs; i++)
		{
			player->bullets[i].SetPosition(player->rect.x, player->rect.y);
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