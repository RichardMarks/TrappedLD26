#ifndef __TRAPPEDLD26_SWITCH_H__
#define __TRAPPEDLD26_SWITCH_H__

#include <SDL/SDL.h>
#include <vector>

struct SwitchConnection
{
    unsigned id;
    unsigned patha;
    unsigned pathb;
};

class Switch
{
    public:
    static std::vector<Switch> switches;
    static void Add(Switch& switchr);
    static void DrawAll(SDL_Surface* dest);

    unsigned id;
    unsigned posx;
    unsigned posy;
    bool flipped;
    SDL_Rect r;
    std::vector<SwitchConnection> connectionlist;
    Switch();
    void AddConnection(unsigned laserid, unsigned patha, unsigned pathb);
    void Draw(SDL_Surface* dest);
    void Flip();
};

#endif
