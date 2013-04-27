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

static const unsigned WINDOW_WIDTH = 640;
static const unsigned WINDOW_HEIGHT = 640;
static const unsigned GRID_RESX = 20;
static const unsigned GRID_RESY = 20;

struct KBDevice
{
    int count;
    unsigned char* state;
    bool* pressed;
    KBDevice()
    {
        state = SDL_GetKeyState(&count);
        pressed = new bool [count];
        for (signed i = 0; i < count; i++)
        {
            pressed[i] = false;
        }
    }
    ~KBDevice()
    {
        delete[] pressed;
    }
    void Update()
    {
        SDL_PumpEvents();
        state = SDL_GetKeyState(0);
    }
    bool IsDown(unsigned sym)
    {
        return state[sym];
    }
};

enum
{
    Laser_Off = 0,
    Laser_Up,
    Laser_Right,
    Laser_Down,
    Laser_Left
};

struct Laser
{
    static std::vector<Laser> lasers;

    unsigned id;
    unsigned posx;
    unsigned posy;
    unsigned direction;
    unsigned thickness;
    unsigned color;
    SDL_Rect r;
    Laser()
    {
        thickness = 8;
        id = 0;
        posx = 0;
        posy = 0;
        direction = Laser_Off;
    }
    void Draw(SDL_Surface* dest)
    {
        if (direction == Laser_Off || dest == NULL)
        {
            return;
        }
        color = SDL_MapRGB(dest->format, 255, 0, 0);
        switch(direction)
        {
            case Laser_Up:
            {
                // laser goes from origin to the top of the screen
                r.x = posx;
                r.y = 0;
                r.w = thickness;
                r.h = posy;
            } break;
            case Laser_Down:
            {
                // laser goes from origin to bottom of the screen
                r.x = posx;
                r.y = posy;
                r.w = thickness;
                r.h = WINDOW_HEIGHT - posy;
            } break;
            case Laser_Left:
            {
                // laser goes from origin to left edge of the screen
                r.x = 0;
                r.y = posy;
                r.w = posx;
                r.h = thickness;
            } break;
            case Laser_Right:
            {
                // laser goes from origin to right edge of the screen
                r.x = posx;
                r.y = posy;
                r.w = WINDOW_WIDTH - posx;
                r.h = thickness;
            } break;
            default:break;
        }
        SDL_FillRect(dest, &r, color);
    }

    static void Add(int x, int y, int d)
    {
        Laser laser;
        laser.id = Laser::lasers.size();
        laser.posx = x;
        laser.posy = y;
        laser.direction = d;
        Laser::lasers.push_back(laser);
    }

    static void DrawAll(SDL_Surface* dest)
    {
        for (unsigned i = 0; i < Laser::lasers.size(); i++)
        {
            Laser::lasers[i].Draw(dest);
        }
    }
};

std::vector<Laser> Laser::lasers;

struct SwitchConnection
{
    unsigned id;
    unsigned patha;
    unsigned pathb;
};
struct Switch
{
    static std::vector<Switch> switches;

    unsigned id;
    unsigned posx;
    unsigned posy;
    bool flipped;

    std::vector<SwitchConnection> connectionlist;

    Switch()
    {
        id = 0;
        posx = 0;
        posy = 0;
        flipped = false;
    }

    static void Add(Switch& switchr)
    {
        switchr.id = Switch::switches.size();
        Switch::switches.push_back(switchr);
    }

    static void DrawAll(SDL_Surface* dest)
    {
        for (unsigned i = 0; i < Switch::switches.size(); i++)
        {
            Switch::switches[i].Draw(dest);
        }
    }

    void AddConnection(Laser& laser, unsigned patha, unsigned pathb)
    {
        SwitchConnection connection;
        connection.id = laser.id;
        connection.patha = patha;
        connection.pathb = pathb;
        connectionlist.push_back(connection);
    }

    void Draw(SDL_Surface* dest)
    {
        if (dest == NULL)
        {
            return;
        }

        unsigned color = flipped ? SDL_MapRGB(dest->format, 0, 255, 0) : SDL_MapRGB(dest->format, 0, 96, 0);

        SDL_Rect r;
        r.w = 16;
        r.h = 16;
        r.x = posx - 8;
        r.y = posy - 8;
        SDL_FillRect(dest, &r, color);
    }

    void Flip()
    {
        // flip the switch
        flipped = !flipped;

        // all connections should be switched
        for (unsigned i = 0; i < connectionlist.size(); i++)
        {
            SwitchConnection& connection = connectionlist[i];
            Laser& laser = Laser::lasers[connection.id];
            if (laser.direction == connection.patha)
            {
                laser.direction = connection.pathb;
            }
            else
            {
                laser.direction = connection.patha;
            }
        }
    }
};

std::vector<Switch> Switch::switches;

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
    Laser::Add(32, 32, Laser_Up);

    Switch s1;
    s1.AddConnection(Laser::lasers[0], Laser_Up, Laser_Right);
    s1.posx = 64;
    s1.posy = 128;
    Switch::Add(s1);

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
                            s1.Flip();
                        }
                        default: break;
                    }
                }
            }
        }
        keyboard.Update();
        // update
        unsigned currenttime = SDL_GetTicks();
        unsigned deltatime = (unsigned)(currenttime - time);
        time = currenttime;

        // render
        SDL_FillRect(display, &display->clip_rect, 0);

        // draw lasers
        Laser::DrawAll(display);
        // draw switches
        Switch::DrawAll(display);
        // draw player

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
