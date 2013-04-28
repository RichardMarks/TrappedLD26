#include "player.h"
#include "constants.h"
#include "laser.h"
#include "switch.h"
#include "collision.h"
#include "kbdevice.h"

Player* Player::handle = NULL;

Player::Player()
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

void Player::Restart()
{
    posx = (WINDOW_WIDTH - r.w) / 2;
    posy = (WINDOW_HEIGHT - r.h) / 2;
    wingame = false;
    dead = false;
    switchbox = NULL;
    Repos();
}

void Player::Update(unsigned deltatime)
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

void Player::Draw(SDL_Surface* dest)
{
    if (dest == NULL)
    {
        return;
    }
    SDL_FillRect(dest, &r, dead ? SDL_MapRGB(dest->format, 60, 30, 30) : SDL_MapRGB(dest->format, 255, 255, 255));
}

void Player::Repos()
{
    r.x = posx;
    r.y = posy;
}

void Player::MoveUp()
{
    posy -= move;
    Repos();
    Collisions();
}

void Player::Collisions()
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

void Player::MoveDown()
{
    posy += move;
    Repos();
    Collisions();
}

void Player::MoveLeft()
{
    posx -= move;
    Repos();
    Collisions();
}

void Player::MoveRight()
{
    posx += move;
    Repos();
    Collisions();
}

Laser* Player::HitLaser()
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

Switch* Player::HitSwitch()
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

bool Player::OnSwitchbox()
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
