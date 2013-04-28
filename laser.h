#ifndef __TRAPPEDLD26_LASER_H__
#define __TRAPPEDLD26_LASER_H__

#include <SDL/SDL.h>
#include <vector>

enum
{
    Laser_Off = 0,
    Laser_Up = 1,
    Laser_Right = 2,
    Laser_Down = 3,
    Laser_Left = 4
};

class Laser
{
    public:
    static std::vector<Laser> lasers;
    static void Add(int x, int y, int d);
    static void DrawAll(SDL_Surface* dest);
    unsigned id;
    unsigned posx;
    unsigned posy;
    unsigned direction;
    unsigned thickness;
    unsigned color;
    SDL_Rect r, r2;
    Laser();
    void Draw(SDL_Surface* dest);
};

#endif
