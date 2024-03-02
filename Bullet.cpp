#include "Bullet.h"

Bullet::Bullet()
{
}
Bullet::Bullet(int x, int y)
{
    rect.w = 5;
    rect.h = 5;
    rect.x = x;
    rect.y = y;

    Damage = 10;
    Speed = 5;
    State = Ready;
    lifespan = BULLET_LIFE_SPAN;
}
void Bullet::Move(int direction)
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
void Bullet::Reset(int x, int y)
{
    State = Ready;
    lifespan = BULLET_LIFE_SPAN;
    rect.x = x;
    rect.y = y;
}
void Bullet::Draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}