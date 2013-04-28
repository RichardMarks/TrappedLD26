#include "levels.h"
#include "switch.h"
#include "laser.h"

typedef void (*levelfunction)();
static levelfunction levels[] =
{
    &Level1, &Level2, &Level3, &Level4, &Level5
};

bool LoadLevel(unsigned level)
{
    if (level >= LEVEL_COUNT)
    {
        return false;
    }

    Laser::lasers.clear();
    Switch::switches.clear();
    levels[level]();
    return true;
}

// helpers
unsigned SwitchX(unsigned xpos) { return 128 + (xpos* 128); }
unsigned SwitchY(unsigned ypos) { return 128 + (ypos* 128); }

void Level2()
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
    Laser::Map(lasermap);

    /*
    unsigned switchmap[] = {
        0,0,1,0,
        0,0,0,0,
        0,1,0,0,
        0,0,0,0
    };
    */

    Switch s1, s2;
    s1.posx = SwitchX(1);
    s1.posy = SwitchY(2);
    s2.posx = SwitchX(2);
    s2.posy = SwitchY(0);


    s1.AddConnection(1, 8);
    s2.AddConnection(2, 1|4);
    s2.AddConnection(3, 1|2);
    s2.AddConnection(1, 4);

    Switch::Add(s1);
    Switch::Add(s2);
}


void Level3(){}
void Level4(){}
void Level5(){}
