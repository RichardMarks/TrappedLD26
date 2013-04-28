#include "switch.h"
#include "constants.h"
#include "laser.h"

std::vector<Switch> Switch::switches;

unsigned SwitchX(unsigned xpos) { return 128 + (xpos* 128); }
unsigned SwitchY(unsigned ypos) { return 128 + (ypos* 128); }

Switch::Switch()
{
    id = 0;
    posx = 0;
    posy = 0;
    flipped = false;
    r.w = 16;
    r.h = 16;
}

void Switch::Add(Switch& switchr)
{
    switchr.id = Switch::switches.size();
    Switch::switches.push_back(switchr);
}

void Switch::DrawAll(SDL_Surface* dest)
{
    for (unsigned i = 0; i < Switch::switches.size(); i++)
    {
        Switch::switches[i].Draw(dest);
    }
}

void Switch::AddConnection(unsigned laserid, unsigned pathb)
{
    SwitchConnection connection;
    connection.id = laserid;
    connection.patha = Laser::lasers[laserid].direction;
    connection.pathb = pathb;
    connectionlist.push_back(connection);
}

void Switch::Draw(SDL_Surface* dest)
{
    if (dest == NULL)
    {
        return;
    }

    unsigned color = flipped ? SDL_MapRGB(dest->format, 0, 255, 0) : SDL_MapRGB(dest->format, 0, 96, 0);

    r.x = posx - (r.w / 2);
    r.y = posy - (r.h / 2);
    SDL_FillRect(dest, &r, color);
}

void Switch::Flip()
{
    // flip the switch
    flipped = !flipped;

    // all connections should be switched
    for (unsigned i = 0; i < connectionlist.size(); i++)
    {
        SwitchConnection& connection = connectionlist[i];
        Laser& laser = Laser::lasers[connection.id];
        if (flipped)
        {
            laser.direction = connection.pathb;
        }
        else
        {
            laser.direction = connection.patha;
        }
    }
}
