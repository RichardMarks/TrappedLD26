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
#include <cmath>
#include "data.h"

static const unsigned WINDOW_WIDTH = 640;
static const unsigned WINDOW_HEIGHT = 640;
static const unsigned GRID_RESX = 20;
static const unsigned GRID_RESY = 20;

struct KBDevice
{
    int count;
    unsigned char* state;
    bool* pressed;
    static KBDevice* handle;
    KBDevice()
    {
        state = SDL_GetKeyState(&count);
        pressed = new bool [count];
        for (signed i = 0; i < count; i++)
        {
            pressed[i] = false;
        }
        KBDevice::handle = this;
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

KBDevice* KBDevice::handle = NULL;

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
    SDL_Rect r, r2;
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
        r2.x = posx;// - thickness;
        r2.y = posy;// - thickness;
        r2.w = thickness * 2;
        r2.h = thickness * 2;

        SDL_FillRect(dest, &r, color);
        SDL_FillRect(dest, &r2, SDL_MapRGB(dest->format, 90, 90, 90));
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
    SDL_Rect r;

    std::vector<SwitchConnection> connectionlist;

    Switch()
    {
        id = 0;
        posx = 0;
        posy = 0;
        flipped = false;
        r.w = 16;
        r.h = 16;
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

        r.x = posx - (r.w / 2);
        r.y = posy - (r.h / 2);
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

struct Player
{
    static Player* handle;
    unsigned posx;
    unsigned posy;
    unsigned speed;
    unsigned move;
    bool wingame;
    bool dead;
    SDL_Rect r;
    Switch* switchbox;

    Player()
    {
        r.w = 24;
        r.h = 48;
        posx = (WINDOW_WIDTH - r.w) / 2;
        posy = (WINDOW_HEIGHT - r.h) / 2;
        speed = 4;
        wingame = false;
        Player::handle = this;
        dead = false;
        switchbox = NULL;
        Repos();
    }

    void Restart()
    {
        posx = (WINDOW_WIDTH - r.w) / 2;
        posy = (WINDOW_HEIGHT - r.h) / 2;
        wingame = false;
        dead = false;
        switchbox = NULL;
        Repos();
    }

    void Update(unsigned deltatime)
    {
        if (wingame || dead)
        {
            return;
        }

        move = speed * (deltatime / 30);
        KBDevice& keyboard = *KBDevice::handle;
        if (keyboard.IsDown(SDLK_UP) || keyboard.IsDown(SDLK_w)) { MoveUp(); }
        if (keyboard.IsDown(SDLK_DOWN) || keyboard.IsDown(SDLK_s)) { MoveDown(); }
        if (keyboard.IsDown(SDLK_LEFT) || keyboard.IsDown(SDLK_a)) { MoveLeft(); }
        if (keyboard.IsDown(SDLK_RIGHT) || keyboard.IsDown(SDLK_d)) { MoveRight(); }
        //r.x = posx;
        //r.y = posy;

        if (!OnSwitchbox())
        {
            switchbox = NULL;
        }

        if (posx <= 16 || posx + r.w >= WINDOW_WIDTH-16 || posy <= 16 || posy + r.h >= WINDOW_HEIGHT-16)
        {
            wingame = true;
            return;
        }

    }

    void Draw(SDL_Surface* dest)
    {
        if (dest == NULL)
        {
            return;
        }
        SDL_FillRect(dest, &r, dead ? SDL_MapRGB(dest->format, 60, 30, 30) : SDL_MapRGB(dest->format, 255, 255, 255));
    }

    void Repos()
    {
        r.x = posx;
        r.y = posy;
    }

    void MoveUp()
    {
        posy -= move;
        Repos();
        Collisions();
    }

    void Collisions()
    {
        Switch* s = HitSwitch();
        if (s != NULL)
        {
            s->Flip();
        }

        Laser* laser = HitLaser();
        if (laser != NULL)
        {
            dead = true;
        }
    }

    void MoveDown()
    {
        posy += move;
        Repos();
        Collisions();
    }

    void MoveLeft()
    {
        posx -= move;
        Repos();
        Collisions();
    }

    void MoveRight()
    {
        posx += move;
        Repos();
        Collisions();
    }

    Laser* HitLaser()
    {
        // scan laser list for any collisions
        for (unsigned li = 0; li < Laser::lasers.size(); li++)
        {
            Laser& l = Laser::lasers[li];
            if (RectsCollide(r, l.r))
            {
                return &l;
            }
        }
        return NULL;
    }

    Switch* HitSwitch()
    {
        // scan switches list for any collisions
        for (unsigned si = 0; si < Switch::switches.size(); si++)
        {
            Switch& s = Switch::switches[si];
            if (switchbox != NULL && &s == switchbox)
            {
                continue;
            }

            if (RectsCollide(r, s.r))
            {
                switchbox = &s;
                return &s;
            }
        }
        return NULL;
    }

    bool OnSwitchbox()
    {
        if (switchbox == NULL)
        {
            return false;
        }

        if (!RectsCollide(r, switchbox->r))
        {
            return false;
        }
        return true;
    }
};

Player* Player::handle = NULL;

enum
{
    Title_State = 0,
    Instructions_State,
    Play_State,
    Credits_State
};

struct Game
{
    static Game* handle;
    unsigned state;
    Player player;
    Game()
    {
        state = Title_State;
        Game::handle = this;
    }
};
Game* Game::handle = NULL;

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
    Laser::Add(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) + 128, Laser_Right);

    Switch s1;
    s1.AddConnection(Laser::lasers[0], Laser_Up, Laser_Right);
    s1.AddConnection(Laser::lasers[1], Laser_Right, Laser_Left);
    s1.posx = 64;
    s1.posy = WINDOW_HEIGHT / 2;
    Switch::Add(s1);

    Switch s2;
    s2.AddConnection(Laser::lasers[1], Laser_Right, Laser_Down);
    s2.posx = (WINDOW_WIDTH / 2) + 128;
    s2.posy = WINDOW_HEIGHT / 2;
    Switch::Add(s2);

    //Player player;

    Game game;
    Player& player = Game::handle->player;

    unsigned edgecolor = SDL_MapRGB(display->format, 160, 160, 160);
    SDL_Rect topedge;
    unsigned edgewidth = 16;
    topedge.x = 0;
    topedge.y = 0;
    topedge.w = WINDOW_WIDTH;
    topedge.h = edgewidth;
    SDL_Rect bottomedge;
    bottomedge.x = 0;
    bottomedge.y = WINDOW_HEIGHT - edgewidth;
    bottomedge.w = WINDOW_WIDTH;
    bottomedge.h = edgewidth;
    SDL_Rect leftedge;
    leftedge.x = 0;
    leftedge.y = edgewidth;
    leftedge.w = edgewidth;
    leftedge.h = WINDOW_HEIGHT - (edgewidth * 2);
    SDL_Rect rightedge;
    rightedge.x = WINDOW_WIDTH - edgewidth;
    rightedge.y = edgewidth;
    rightedge.w = edgewidth;
    rightedge.h = WINDOW_HEIGHT - (edgewidth * 2);

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
                            if (game.state == Title_State)
                            {
                                game.state = Instructions_State;
                            }
                            else if (game.state == Instructions_State)
                            {
                                game.state = Play_State;
                            }
                            else if (game.state == Play_State)
                            {
                                if (player.wingame || player.dead)
                                {
                                    game.state = Credits_State;
                                }
                            }
                            else if (game.state == Credits_State)
                            {
                                player.Restart();
                                game.state = Title_State;
                            }
                        }
                        /*
                        case SDLK_r:
                        {
                            if (player.wingame || player.dead)
                            {
                                player.Restart();
                            }
                        }
                        */
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

        if (game.state == Play_State)
        {
            player.Update(deltatime);
        }

        // render
        SDL_FillRect(display, &display->clip_rect, 0);

        if (game.state == Play_State)
        {
            // draw lasers
            Laser::DrawAll(display);
            // draw switches
            Switch::DrawAll(display);
            // draw player
            player.Draw(display);
            // draw edges
            SDL_FillRect(display, &topedge, edgecolor);
            SDL_FillRect(display, &bottomedge, edgecolor);
            SDL_FillRect(display, &leftedge, edgecolor);
            SDL_FillRect(display, &rightedge, edgecolor);
        }
        else if (game.state == Title_State)
        {
            RenderPackedPackage(titlepackage, 32, display, SDL_MapRGB(display->format, 255, 255, 255));
        }
        else if (game.state == Instructions_State)
        {
            RenderPackedPackage(instructionspackage, 32, display, SDL_MapRGB(display->format, 255, 255, 255));
        }
        else if (game.state == Credits_State)
        {
            RenderPackedPackage(creditspackage, 32, display, SDL_MapRGB(display->format, 255, 255, 255));
        }

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
