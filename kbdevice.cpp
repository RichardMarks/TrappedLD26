#include "kbdevice.h"

KBDevice* KBDevice::handle = NULL;

KBDevice::KBDevice()
{
    state = SDL_GetKeyState(&count);
    pressed = new bool [count];
    for (signed i = 0; i < count; i++)
    {
        pressed[i] = false;
    }
    KBDevice::handle = this;
}

KBDevice::~KBDevice()
{
    delete[] pressed;
}

void KBDevice::Update()
{
    SDL_PumpEvents();
    state = SDL_GetKeyState(0);
}

bool KBDevice::IsDown(unsigned sym)
{
    return state[sym];
}
