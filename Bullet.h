#pragma once

#include <SDL2/SDL.h>

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

#define BULLET_LIFE_SPAN 100

struct Bullet
{
    SDL_Rect rect;
    int Damage;
    int Speed;
    int State;
    bool isNPCs;
    int direction;
    int lifespan;

    Bullet();
    Bullet(int x, int y);
    void Move(int direction);
    void Reset(int x, int y);
    void Draw(SDL_Renderer *renderer);
};