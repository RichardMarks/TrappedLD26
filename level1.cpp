#include "levels.h"
#include "switch.h"
#include "laser.h"

void Level1()
{
    /*
        0 - off
        1 - up
        2 - right
        4 - down
        8 - left
    */
    unsigned lasermap[] = {
        0|0, 0|0, 0|0, 0|0, 0|0,
        0|0, 2|4, 0|0, 4|8, 0|0,
        0|0, 0|0, 0|0, 0|0, 0|0,
        0|0, 1|2, 0|0, 1|8, 0|0,
        0|0, 0|0, 0|0, 0|0, 0|0
    };
    Laser::Map(lasermap);

    Switch s1;
    s1.posx = SwitchX(1);
    s1.posy = SwitchY(1);

    s1.AddConnection(0, 1|8|4);
    s1.AddConnection(1, 1|2|8);
    s1.AddConnection(2, 8|4);
    s1.AddConnection(3, 4|2|8);

    Switch::Add(s1);
}
