#include "Entity.h"

Entity::Entity()
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

Player::Player()
{
    rect.x = rand() % 800;
    rect.y = rand() % 600;

    for (int i = 0; i < NUM_OF_BULLETs; i++)
    {
        bullets[i].isNPCs = false;
    }
}
void Player::Move()
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
void Player::SetPostion(int x, int y)
{
    rect.x = x;
    rect.y = y;
}
void Player::Draw(SDL_Renderer *renderer, int R, int G, int B)
{
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    SDL_RenderFillRect(renderer, &rect);
}
void Player::Draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
void Player::Shoot()
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

NPC::NPC()
{
    rect.x = rand() % 800;
    rect.y = rand() % 600;
}
void NPC::Move()
{
    // Move the NPC (PathFinding Algo):
}
void NPC::SetPostion(int x, int y)
{
    rect.x = x;
    rect.y = y;
}
void NPC::Draw(SDL_Renderer *renderer, int R, int G, int B)
{
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    SDL_RenderFillRect(renderer, &rect);
}
void NPC::Draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
void NPC::Shoot()
{
}