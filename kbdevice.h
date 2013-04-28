#ifndef __TRAPPEDLD26_KBDEVICE_H__
#define __TRAPPEDLD26_KBDEVICE_H__

#include <SDL/SDL.h>

class KBDevice
{
    public:
    int count;
    unsigned char* state;
    bool* pressed;
    static KBDevice* handle;
    KBDevice();
    ~KBDevice();
    void Update();
    bool IsDown(unsigned sym);
};

#endif
