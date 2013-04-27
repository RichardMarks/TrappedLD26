// Trapped LD26
// A Ludum Dare 26 Compo Entry by Richard Marks (ccpsceo@gmail.com)
// © Copyright 2013, Richard Marks, Bang Bang Attack Studios
// www.ccpssolutions.com | www.bangbangattackstudios.com
// ================================================================

/*
    Objective: Reach the edge of the screen to win the game.

    Summary:

    In Trapped LD26, you must attempt to reach the edge of the screen.
    There are laser beams (red lines) that are blocking your way.
    If you touch a laser beam, you die.
    There are switches (green boxes) that you must walk on to change
    the direction that the lasers are pointing. Careful, some switches
    control multiple lasers, and you may get fried!

    Controls:

    WASD, Arrow Keys to move player
    ESC to quit
*/

#include <SDL/SDL.h>

static const unsigned WINDOW_WIDTH = 640;
static const unsigned WINDOW_HEIGHT = 640;
static const unsigned GRID_RESX = 20;
static const unsigned GRID_RESY = 20;

struct KBDevice
{
    int count;
    unsigned char* state;
    bool* pressed;
    KBDevice()
    {
        state = SDL_GetKeyState(&count);
        pressed = new bool [count];
        for (signed i = 0; i < count; i++)
        {
            pressed[i] = false;
        }
    }
    ~KBDevice()
    {
        delete[] pressed;
    }
    void Update()
    {
        SDL_PumpEvents();
        state = SDL_GetKeyState(0);
    }
};

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return -1;
    }
    SDL_Surface* display = NULL;
    if ((display = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
    {
        return -1;
    }

    SDL_WM_SetCaption("Trapped LD26", NULL);
    KBDevice keyboard;

    SDL_Event sdlevent;
    bool running = true;
    unsigned time = 0;
    unsigned waittime = 1000.0 / 30;
    unsigned framestarttime = 0;

    // init

    // main loop
    time = SDL_GetTicks();
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
                        default: break;
                    }
                }
            }
        }
        keyboard.Update();
        // update
        unsigned currenttime = SDL_GetTicks();
        unsigned deltatime = (unsigned)(currenttime - time);
        time = currenttime;

        // render
        SDL_FillRect(display, &display->clip_rect, 0);

        // draw lasers
        // draw switches
        // draw player

        SDL_Flip(display);
        deltatime = (SDL_GetTicks() - framestarttime);
        if (deltatime < waittime)
        {
            SDL_Delay((Uint32)waittime - (SDL_GetTicks() - framestarttime));
        }
        framestarttime = SDL_GetTicks();
    }

    // cleanup
    SDL_Quit();
    return 0;
}
