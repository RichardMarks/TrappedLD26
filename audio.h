#ifndef __TRAPPEDLD26_AUDIO_H__
#define __TRAPPEDLD26_AUDIO_H__

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <vector>

struct Clip
{
    unsigned id;
    unsigned volume;
    Mix_Chunk* chunk;
};

class Audio
{
    public:
    static std::vector<Clip> clips;
    static Audio* handle;
    static signed LoadClip(const char* filename);
    static void Play(unsigned clipid);
    bool ready;
    Audio();
    ~Audio();
};

#endif
