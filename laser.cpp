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
    for (unsigned i = 0; i < Laser::lasers.size(); i++)
    {
        Laser::lasers[i].Draw(dest);
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
void Laser::Draw(SDL_Surface* dest)
{
    if (direction == Laser_Off || dest == NULL)
    {
        return;
    }
    color = SDL_MapRGB(dest->format, 255, 0, 0);
    switch(direction)
    {
        case Laser_Up:
        {
            // laser goes from origin to the top of the screen
            r.x = posx;
            r.y = 0;
            r.w = thickness;
            r.h = posy;
        } break;
        case Laser_Down:
        {
            // laser goes from origin to bottom of the screen
            r.x = posx;
            r.y = posy;
            r.w = thickness;
            r.h = WINDOW_HEIGHT - posy;
        } break;
        case Laser_Left:
        {
            // laser goes from origin to left edge of the screen
            r.x = 0;
            r.y = posy;
            r.w = posx;
            r.h = thickness;
        } break;
        case Laser_Right:
        {
            // laser goes from origin to right edge of the screen
            r.x = posx;
            r.y = posy;
            r.w = WINDOW_WIDTH - posx;
            r.h = thickness;
        } break;
        default:break;
    }
    r2.x = posx;// - thickness;
    r2.y = posy;// - thickness;
    r2.w = thickness * 2;
    r2.h = thickness * 2;

    SDL_FillRect(dest, &r, color);
    SDL_FillRect(dest, &r2, SDL_MapRGB(dest->format, 90, 90, 90));
}
