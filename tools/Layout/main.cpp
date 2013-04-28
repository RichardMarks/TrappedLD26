// Trapped LD26: Tools: Layout
// A Ludum Dare 26 Compo Entry by Richard Marks (ccpsceo@gmail.com)
// © Copyright 2013, Richard Marks, Bang Bang Attack Studios
// www.ccpssolutions.com | www.bangbangattackstudios.com
// ================================================================

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <vector>
#include <iostream>
#include <cmath>

static const unsigned WINDOW_WIDTH = 640;
static const unsigned WINDOW_HEIGHT = 640;
static const unsigned GRID_RESX = 20;
static const unsigned GRID_RESY = 20;

enum {
    Laser_Off, Laser_Up, Laser_Right, Laser_Down, Laser_Left

};

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return -1;
    }
    SDL_Surface* display = NULL;
    if ((display = SDL_SetVideoMode(1280, 640, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
    {
        return -1;
    }


    SDL_Rect r;
    for (unsigned y = 64; y < WINDOW_HEIGHT - 60; y+= 128)
    {
        for (unsigned x = 64; x < WINDOW_WIDTH - 60; x+= 128)
        {
            r.x = x-8;
            r.y = y-8;
            r.w = 16;
            r.h = 16;
            SDL_FillRect(display, &r, SDL_MapRGB(display->format, 255, 0, 0));
        }
    }

    for (unsigned y = 128; y < WINDOW_HEIGHT - 120; y+= 128)
    {
        for (unsigned x = 128; x < WINDOW_WIDTH - 120; x+= 128)
        {
            r.x = x-8;
            r.y = y-8;
            r.w = 16;
            r.h = 16;
            SDL_FillRect(display, &r, SDL_MapRGB(display->format, 0, 255, 0));
        }
    }




    SDL_Event sdlevent;
    bool running = true;
    while(running)
    {
        while(SDL_PollEvent(&sdlevent))
        {
            switch(sdlevent.type)
            {
                case SDL_QUIT: { running = false; } break;
                case SDL_KEYDOWN: {
                    switch(sdlevent.key.keysym.sym)
                    {
                        case SDLK_ESCAPE: { running = false; } break;
                        default:break;
                    }
                }
            }
        }

        SDL_Flip(display);
        SDL_Delay(100);
    }

    // cleanup
    SDL_Quit();
    return 0;
}
