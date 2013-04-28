#include "audio.h"

std::vector<Clip> Audio::clips;
Audio* Audio::handle = NULL;

signed Audio::LoadClip(const char* filename)
{
    Mix_Chunk* chunk = NULL;
    chunk = Mix_LoadWAV(filename);
    if (chunk == NULL)
    {
        return -1;
    }
    Clip clip;
    clip.chunk = chunk;
    clip.volume = MIX_MAX_VOLUME;
    clip.id = Audio::clips.size();
    Audio::clips.push_back(clip);
    return clip.id;
}

void Audio::Play(unsigned clipid)
{
    if (clipid >= Audio::clips.size())
    {
        return;
    }
    Clip& clip = Audio::clips[clipid];

    Mix_VolumeChunk(clip.chunk, clip.volume);
    Mix_PlayChannel(-1, clip.chunk, 0);
}

Audio::Audio()
{
    ready = true;
    if (0 != Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
    {
        ready = false;
    }
}

Audio::~Audio()
{
    for (unsigned i = 0; i < Audio::clips.size(); i++)
    {
       Clip& clip = Audio::clips[i];
       if (clip.chunk != NULL)
       {
           Mix_FreeChunk(clip.chunk);
           clip.chunk = NULL;
       }
    }
    Audio::clips.clear();
    Mix_CloseAudio();
}
