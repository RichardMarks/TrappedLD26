
#include "collision.h"

bool RectsCollide(SDL_Rect& first, SDL_Rect& second)
{
    unsigned left1, left2;
    unsigned right1, right2;
    unsigned top1, top2;
    unsigned bottom1, bottom2;

    left1 = first.x;
    right1 = first.x + first.w;
    top1 = first.y;
    bottom1 = first.y + first.h;

    left2 = second.x;
    right2 = second.x + second.w;
    top2 = second.y;
    bottom2 = second.y + second.h;

    if (bottom1 <= top2 || top1 >= bottom2 || right1 <= left2 || left1 >= right2)
    {
        return false;
    }

    return true;
}

