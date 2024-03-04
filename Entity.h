#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "Bullet.h"
#include "CollisionSystem.h"

#define NUM_OF_BULLETs 4

struct Entity
{
    SDL_Rect rect;
    Frame frame;
    int Health;
    int Speed;
    int Direction;
    int DamageOut;
    int DamageIn;
    bool Slowed;
    bool Moving;
    std::vector<Bullet> bullets;

    Entity();
    virtual void Move() = 0;
    virtual void SetPostion(int x, int y) = 0;
    virtual void Draw(SDL_Renderer *renderer, int R, int G, int B) = 0;
    virtual void Draw(SDL_Renderer *renderer) = 0;
    virtual void Shoot() = 0;
    virtual void updateFrame();
};

struct Player : public Entity
{
    Player();
    void Move() override;
    void SetPostion(int x, int y) override;
    void Draw(SDL_Renderer *renderer, int R, int G, int B) override;
    void Draw(SDL_Renderer *renderer) override;
    void Shoot() override;
};

struct NPC : public Entity
{
    NPC();
    void Move() override;
    void SetPostion(int x, int y) override;
    void Draw(SDL_Renderer *renderer, int R, int G, int B) override;
    void Draw(SDL_Renderer *renderer) override;
    void Shoot() override;
};