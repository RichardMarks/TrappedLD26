#include "game.h"
#include "constants.h"
#include "laser.h"
#include "switch.h"
#include <cmath>

Game* Game::handle = NULL;

Game::Game()
{
    state = Title_State;
    Game::handle = this;
    potato = false;
}

void Game::Reset()
{
    SetupLasers();
    SetupSwitches();
}

void Game::NextState()
{
    if (state == Title_State)
    {
        state = Instructions_State;
    }
    else if (state == Instructions_State)
    {
        state = Play_State;
    }
    else if (state == Win_State)
    {
        state = Credits_State;
    }
    else if (state == Gameover_State)
    {
        state = Credits_State;
    }
    else if (state == Potato_State)
    {
        state = Play_State;
        if (player.dead)
        {
            state = Gameover_State;
        }
    }
    else if (state == Play_State)
    {
        if (player.dead)
        {
            state = Gameover_State;
        }
    }
    else if (state == Credits_State)
    {
        player.Restart();
        state = Title_State;
        potato = false;
    }
}

void Game::SetupLasers()
{
    /*
        0 - off
        1 - up
        2 - right
        4 - down
        8 - left
    */
    unsigned lasermap[] = {
        2|4, 0|0, 0|0, 0|0, 0|0,
        0|0, 0|0, 0|0, 4|8, 0|0,
        0|0, 0|0, 0|0, 0|0, 0|0,
        0|0, 1|2, 0|0, 0|0, 0|0,
        0|0, 0|0, 0|0, 0|0, 1|8
    };

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
                if (key & 1) { Laser::Add(xpos, ypos, Laser_Up); }
                if (key & 2) { Laser::Add(xpos, ypos, Laser_Right); }
                if (key & 4) { Laser::Add(xpos, ypos, Laser_Down); }
                if (key & 8) { Laser::Add(xpos, ypos, Laser_Left); }
            }
        }
    }
}

void Game::SetupSwitches()
{
    unsigned switchmap[] = {
        0,0,1,0,
        0,0,0,0,
        0,1,0,0,
        0,0,0,0
    };

    unsigned connectioncountmap[] = {
        0,0,1,0,
        0,0,0,0,
        0,2,0,0,
        0,0,0,0
    };

    for (unsigned y = 0; y < 4; y++)
    {
        unsigned ypos = 128 + (y * 128);
        for (unsigned x = 0; x < 4; x++)
        {
            unsigned xpos = 128 + (x * 128);
            unsigned i = x + (y*4);
            unsigned key = switchmap[i];
            if (key > 0)
            {
                Switch s;
                s.posx = xpos;
                s.posy = ypos;
                Switch::Add(s);
            }
        }
    }

/*
    Switch s1;
    s1.AddConnection(0, Laser_Up, Laser_Right);
    s1.AddConnection(1, Laser_Right, Laser_Left);
    s1.posx = 64;
    s1.posy = WINDOW_HEIGHT / 2;
    Switch::Add(s1);

    Switch s2;
    s2.AddConnection(1, Laser_Right, Laser_Down);
    s2.posx = (WINDOW_WIDTH / 2) + 128;
    s2.posy = WINDOW_HEIGHT / 2;
    Switch::Add(s2);
*/
}


void RenderPackedPackage(unsigned* package, unsigned packagelength, SDL_Surface* dest, unsigned color)
{
    std::vector<unsigned> bitmasks;
    for (unsigned i = 0; i < 32; i++)
    {
        bitmasks.push_back(powf(2, 31 - i));
    }

    SDL_Rect block;
    block.w = 20;
    block.h = 20;
    for (unsigned y = 0; y < packagelength; y++)
    {
        for (unsigned x = 0; x < bitmasks.size(); x++)
        {
            if (package[y] & bitmasks[x])
            {
                block.x = x * block.w;
                block.y = y * block.h;
                SDL_FillRect(dest, &block, color);
            }
        }
    }
}
