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
#include <vector>
#include <cmath>
#include "data.h"
#include "constants.h"
#include "kbdevice.h"
#include "laser.h"
#include "switch.h"
#include "player.h"
#include "game.h"

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

    Game game;
    game.Reset();
    Player& player = Game::handle->player;

    unsigned edgecolor = SDL_MapRGB(display->format, 160, 160, 160);
    SDL_Rect topedge;
    unsigned edgewidth = 16;
    topedge.x = 0;
    topedge.y = 0;
    topedge.w = WINDOW_WIDTH;
    topedge.h = edgewidth;
    SDL_Rect bottomedge;
    bottomedge.x = 0;
    bottomedge.y = WINDOW_HEIGHT - edgewidth;
    bottomedge.w = WINDOW_WIDTH;
    bottomedge.h = edgewidth;
    SDL_Rect leftedge;
    leftedge.x = 0;
    leftedge.y = edgewidth;
    leftedge.w = edgewidth;
    leftedge.h = WINDOW_HEIGHT - (edgewidth * 2);
    SDL_Rect rightedge;
    rightedge.x = WINDOW_WIDTH - edgewidth;
    rightedge.y = edgewidth;
    rightedge.w = edgewidth;
    rightedge.h = WINDOW_HEIGHT - (edgewidth * 2);

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
                        case SDLK_SPACE: {
                            game.NextState();
                        }
                        default: break;
                    }
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    if (sdlevent.button.button == SDL_BUTTON_LEFT)
                    {
                        game.NextState();
                    }
                }
                case SDL_ACTIVEEVENT:
                {
                    Uint8 aestate = sdlevent.active.state;
                    if (aestate & SDL_APPACTIVE)
                    {
                        if (sdlevent.active.gain)
                        {
                            if (game.potato)
                            {
                                game.state = Potato_State;
                            }
                        }
                        else
                        {
                            if (game.state == Play_State)
                            {
                                game.potato = true;
                            }
                        }
                    }
                }
            }
        }
        keyboard.Update();
        // update
        unsigned currenttime = SDL_GetTicks();
        unsigned deltatime = (unsigned)(currenttime - time);
        time = currenttime;

        if (game.state == Play_State)
        {
            player.Update(deltatime);
            if (player.wingame)
            {
                game.state = Win_State;
            }
        }

        // render
        SDL_FillRect(display, &display->clip_rect, 0);

        if (game.state == Play_State)
        {
            // draw lasers
            Laser::DrawAll(display);
            // draw switches
            Switch::DrawAll(display);
            // draw player
            player.Draw(display);
            // draw edges
            SDL_FillRect(display, &topedge, edgecolor);
            SDL_FillRect(display, &bottomedge, edgecolor);
            SDL_FillRect(display, &leftedge, edgecolor);
            SDL_FillRect(display, &rightedge, edgecolor);
        }
        else if (game.state == Title_State)
        {
            RenderPackedPackage(titlepackage, 32, display, SDL_MapRGB(display->format, 255, 255, 255));
        }
        else if (game.state == Instructions_State)
        {
            RenderPackedPackage(instructionspackage, 32, display, SDL_MapRGB(display->format, 255, 255, 255));
        }
        else if (game.state == Credits_State)
        {
            RenderPackedPackage(creditspackage, 32, display, SDL_MapRGB(display->format, 255, 255, 255));
        }
        else if (game.state == Win_State)
        {
            RenderPackedPackage(winpackage, 32, display, SDL_MapRGB(display->format, 255, 255, 255));
        }
        else if (game.state == Gameover_State)
        {
            RenderPackedPackage(gameoverpackage, 32, display, SDL_MapRGB(display->format, 255, 255, 255));
        }
        else if (game.state == Potato_State)
        {
            RenderPackedPackage(potatopackage, 32, display, SDL_MapRGB(display->format, 202, 137, 40));
        }

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
