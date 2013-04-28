#include "game.h"
#include "constants.h"
#include "laser.h"
#include "switch.h"
#include "levels.h"
#include <cmath>

Game* Game::handle = NULL;

Game::Game()
{
    state = Title_State;
    Game::handle = this;
    Reset();
}

void Game::Save()
{
    FILE* fp = fopen("trappedld26save.data", "wb");
    if (fp != NULL)
    {
        for (unsigned i = 0; i < 7; i++)
        {
            unsigned n = 0xCADE6387 + (rand() % 0xABCD8756);
            fwrite(&n, sizeof(unsigned), 1, fp);
        }
        fwrite(&level, sizeof(unsigned), 1, fp);
        fwrite(&player.posx, sizeof(unsigned), 1, fp);
        fwrite(&player.posy, sizeof(unsigned), 1, fp);
        fwrite(&level, sizeof(unsigned), 1, fp);
        for (unsigned i = 0; i < 6; i++)
        {
            unsigned n = 0xCADE6387 + (rand() % 0xABCD8756);
            fwrite(&n, sizeof(unsigned), 1, fp);
        }
        for (unsigned i = 0; i < Switch::switches.size(); i++)
        {
            unsigned char n = Switch::switches[i].flipped ? 1 : 0;
            fwrite(&n, sizeof(unsigned char), 1, fp);
        }
        fclose(fp);
    }
}

void Game::Load()
{
    player.Restart();
    FILE* fp = fopen("trappedld26save.data", "rb");
    if (fp != NULL)
    {
        fseek(fp, 7 * sizeof(unsigned), SEEK_SET);
        fread(&level, sizeof(unsigned), 1, fp);
        fread(&player.posx, sizeof(unsigned), 1, fp);
        fread(&player.posy, sizeof(unsigned), 1, fp);
        player.Repos();
        if (!LoadLevel(level))
        {
            fclose(fp);
            Reset();
            return;
        }
        for (unsigned i = 0; i < Switch::switches.size(); i++)
        {
            unsigned char n;
            fread(&n, sizeof(unsigned char), 1, fp);
            if (n > 0)
            {
                Switch::switches[i].Flip();
            }
        }


        fclose(fp);
    }

}

void Game::RestartLevel()
{
    player.Restart();
    if (!LoadLevel(level))
    {
        Reset();
    }
}

void Game::Reset()
{
    level = 0;
    potato = false;
    player.Restart();
    state = Title_State;
}

void Game::NextState()
{
    switch(state)
    {
        case Title_State:
        {
            state = Instructions_State;
        } break;

        case Instructions_State:
        {
            state = Play_State;
            if (!LoadLevel(level))
            {
                Reset();
            }
        } break;

        case Play_State:
        {
            if (player.dead)
            {
                state = Gameover_State;
            }
        } break;

        case Credits_State:
        {
            Reset();
        } break;

        case Win_State:
        {
            level++;
            if (!LoadLevel(level))
            {
                state = Gameover_State;
            }
            else
            {
                player.Restart();
                state = Play_State;
            }
        } break;

        case Gameover_State:
        {
            state = Credits_State;
        } break;

        case Potato_State:
        {
            state = Play_State;
            if (player.dead)
            {
                state = Gameover_State;
            }
        } break;
    }
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
