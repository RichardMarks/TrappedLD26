#include "laser.h"
#include "constants.h"

std::vector<Laser> Laser::lasers;

void Laser::Add(int x, int y, int d)
{
    Laser laser;
    laser.id = Laser::lasers.size();
    laser.posx = x;
    laser.posy = y;
    laser.direction = d;
    Laser::lasers.push_back(laser);
}

void Laser::DrawAll(SDL_Surface* dest)
{
    unsigned i;
    for (i = 0; i < Laser::lasers.size(); i++)
    {
        Laser::lasers[i].DrawBeams(dest);
    }
    for (i = 0; i < Laser::lasers.size(); i++)
    {
        Laser::lasers[i].DrawBoxes(dest);
    }
}

void Laser::Map(unsigned* lasermap)
{
    Laser::lasers.clear();
    for (unsigned y = 0; y < 5; y++)
    {
        unsigned ypos = 64 + (y * 128);
        for (unsigned x = 0; x < 5; x++)
        {
            unsigned xpos = 64 + (x * 128);
            unsigned i = x + (y*5);
            unsigned key = lasermap[i];
            if (key > 0)
            {
                Laser::Add(xpos, ypos, key);
            }
        }
    }
}

Laser::Laser()
{
    thickness = 8;
    id = 0;
    posx = 0;
    posy = 0;
    direction = Laser_Off;
}

void Laser::DrawBoxes(SDL_Surface* dest)
{
    if (dest == NULL)
    {
        return;
    }

    // turret
    tr.x = posx;
    tr.y = posy;
    tr.w = thickness * 2;
    tr.h = thickness * 2;

    SDL_FillRect(dest, &tr, SDL_MapRGB(dest->format, 90, 90, 90));

    // debug
    #if 1
    SDL_Rect r;
    for (unsigned n = 0; n < id; n++)
    {
        r.x = posx + (8 * n);
        r.y = posy;
        r.w = 2;
        r.h = 2;
        SDL_FillRect(dest, &r, SDL_MapRGB(dest->format, 255, 255, 255));
    }
    #endif
}

void HideRect(SDL_Rect& r)
{
    r.w = 0;
    r.h = 0;
    r.x = -128;
    r.y = -128;
}

void Laser::DrawBeams(SDL_Surface* dest)
{
    if (direction == Laser_Off || dest == NULL)
    {
        return;
    }
    color = SDL_MapRGB(dest->format, 255, 0, 0);

    if (direction & Laser_Up)
    {
        // laser goes from origin to the top of the screen
        lr1.x = posx;
        lr1.y = 0;
        lr1.w = thickness;
        lr1.h = posy;
        SDL_FillRect(dest, &lr1, color);
    }
    else
    {
        HideRect(lr1);
    }

    if (direction & Laser_Right)
    {
        // laser goes from origin to right edge of the screen
        lr2.x = posx;
        lr2.y = posy;
        lr2.w = WINDOW_WIDTH - posx;
        lr2.h = thickness;
        SDL_FillRect(dest, &lr2, color);
    }
    else
    {
        HideRect(lr2);
    }

    if (direction & Laser_Down)
    {
        // laser goes from origin to bottom of the screen
        lr3.x = posx;
        lr3.y = posy;
        lr3.w = thickness;
        lr3.h = WINDOW_HEIGHT - posy;
        SDL_FillRect(dest, &lr3, color);
    }
    else
    {
        HideRect(lr3);
    }

    if (direction & Laser_Left)
    {
        // laser goes from origin to left edge of the screen
        lr4.x = 0;
        lr4.y = posy;
        lr4.w = posx;
        lr4.h = thickness;
        SDL_FillRect(dest, &lr4, color);
    }
    else
    {
        HideRect(lr4);
    }

}
