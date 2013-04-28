#ifndef __TRAPPEDLD26_GAME_H__
#define __TRAPPEDLD26_GAME_H__

#include <SDL/SDL.h>
#include <vector>

#include "player.h"

enum
{
    Title_State = 0,
    Instructions_State,
    Play_State,
    Credits_State,
    Win_State,
    Gameover_State,
    Potato_State
};

class Game
{
    public:
    static Game* handle;
    unsigned state;
    Player player;
    bool potato;
    unsigned level;
    Game();
    void Reset();
    void NextState();
    void SetupLasers();
    void SetupSwitches();
    void Save();
    void Load();
    void RestartLevel();
};
extern void RenderPackedPackage(unsigned* package, unsigned packagelength, SDL_Surface* dest, unsigned color);
#endif
