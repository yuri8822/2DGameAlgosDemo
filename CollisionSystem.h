#pragma once

struct CoOrdinates
{
    int x;
    int y;

    bool operator==(CoOrdinates &rhs)
    {
        return x == rhs.x && y == rhs.y;
    }
};

struct Frame
{
    CoOrdinates coOrdinates[4];

    bool operator==(Frame &rhs)
    {
        // check for a collision:
        if (coOrdinates[0].x >= rhs.coOrdinates[0].x &&
            coOrdinates[1].x <= rhs.coOrdinates[1].x &&
            coOrdinates[0].y >= rhs.coOrdinates[0].y &&
            coOrdinates[2].y <= rhs.coOrdinates[2].y)
        {
            return true;
        }
    }
};