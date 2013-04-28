#ifndef __TRAPPEDLD26_LASER_H__
#define __TRAPPEDLD26_LASER_H__

#include <SDL/SDL.h>
#include <vector>

enum
{
    Laser_Off = 0,
    Laser_Up = 1,
    Laser_Right = 2,
    Laser_Down = 4,
    Laser_Left = 8
};

class Laser
{
    public:
    static std::vector<Laser> lasers;
    static void Add(int x, int y, int d);
    static void DrawAll(SDL_Surface* dest);
    static void Map(unsigned* lasermap);
    unsigned id;
    unsigned posx;
    unsigned posy;
    unsigned direction;
    unsigned thickness;
    unsigned color;
    SDL_Rect tr;
    SDL_Rect lr1, lr2, lr3, lr4;
    Laser();
    void DrawBeams(SDL_Surface* dest);
    void DrawBoxes(SDL_Surface* dest);
};

#endif
