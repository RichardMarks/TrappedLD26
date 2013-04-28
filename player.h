#ifndef __TRAPPEDLD26_PLAYER_H__
#define __TRAPPEDLD26_PLAYER_H__

#include <SDL/SDL.h>
#include <vector>

class Switch;
class Laser;

class Player
{
    public:
    static Player* handle;

    unsigned posx;
    unsigned posy;
    unsigned speed;
    unsigned move;
    bool wingame;
    bool dead;
    SDL_Rect r;
    Switch* switchbox;
    Player();
    void Restart();
    void Update(unsigned deltatime);
    void Draw(SDL_Surface* dest);
    void Repos();
    void MoveUp();
    void Collisions();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    Laser* HitLaser();
    Switch* HitSwitch();
    bool OnSwitchbox();
};



#endif
