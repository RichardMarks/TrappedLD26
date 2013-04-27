// Trapped LD26: Tools: ImagePack
// A Ludum Dare 26 Compo Entry by Richard Marks (ccpsceo@gmail.com)
// © Copyright 2013, Richard Marks, Bang Bang Attack Studios
// www.ccpssolutions.com | www.bangbangattackstudios.com
// ================================================================

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <vector>
#include <iostream>
#include <cmath>

unsigned ReadPixel(SDL_Surface* surface, unsigned x, unsigned y)
{
    unsigned color = 0;
    if (SDL_MUSTLOCK(surface))
    {
        SDL_LockSurface(surface);
    }
    Uint32* pixels = (Uint32*)surface->pixels;
    color = pixels[x + (y * surface->w)];
    if (SDL_MUSTLOCK(surface))
    {
        SDL_UnlockSurface(surface);
    }
    return color;
}

void PackImage(SDL_Surface* source, const char* filename)
{
    std::vector<unsigned> packed;
    std::vector<unsigned> bitmasks;

    if (source->w != 0x20) { return; }
    signed i = 0;
    for (i = 0; i < source->h; i++)
    {
        packed.push_back(0);
    }

    for (i = 0; i < 32; i++)
    {
        bitmasks.push_back(powf(2, 31 - i));
    }

    if (SDL_MUSTLOCK(source))
    {
        SDL_LockSurface(source);
    }
    for (signed y = 0; y < source->h; y++)
    {
        for (signed x = 0; x < source->w; x++)
        {
            unsigned* pixels = (unsigned*)source->pixels;
            unsigned pixel = pixels[x + y * source->w]; //ReadPixel(source, x, y);
            std::cout << "pixel " << pixel << " ";
            if (pixel != 0)
            {
                packed[y] |= bitmasks[x];
            }
        }
    }
    if (SDL_MUSTLOCK(source))
    {
        SDL_UnlockSurface(source);
    }

    FILE* fp = fopen(filename, "w");
    if (fp == NULL)
    {
        return;
    }

    // write packed data to file
    fprintf(fp, "%u\n", packed.size());
    for (i = 0; i < source->h; i++)
    {
        fprintf(fp, "%u,", packed[i]);
    }

    fclose(fp);
}

bool DrawSurface(SDL_Surface* src, SDL_Surface* dest, int x, int y, int x2, int y2, int w, int h)
{
    if (dest == NULL || src == NULL)
    {
        return false;
    }

    SDL_Rect dr;
    dr.x = x;
    dr.y = y;

    SDL_Rect sr;
    sr.x = x2;
    sr.y = y2;
    sr.w = w;
    sr.h = h;

    SDL_BlitSurface(src, &sr, dest, &dr);

    return true;
}

int main(int argc, char* argv[])
{
    if (argc<3)
    {
        std::cout << "ImagePack v1.0\n(C) 2013, Richard Marks (ccpsceo@gmail.com)\nUsage: " << argv[0] << " imagefilename packedfilename\n";
        return 0;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return -1;
    }
    SDL_Surface* display = NULL;
    if ((display = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
    {
        return -1;
    }
    bool viewmode = false;
    if (argv[1][0] == '-' && argv[1][1] == 'v')
    {
        viewmode = true;
    }

    SDL_Surface* img = NULL;
    if (!viewmode)
    {
        SDL_Surface* timg = IMG_Load(argv[1]);

        if (timg != NULL)
        {
            img = SDL_DisplayFormat(timg);
            SDL_FreeSurface(timg);
        }
        else
        {
            return -1;
        }
    }
    else
    {
        std::vector<unsigned> packed;

        FILE* fp = fopen(argv[2], "r");
        if (fp != NULL)
        {
            unsigned count = 0;
            fscanf(fp, "%u\n", &count);
            for (unsigned i = 0; i < count; i++)
            {
                unsigned p;
                fscanf(fp, "%u,", &p);
                packed.push_back(p);
            }
            fclose(fp);
        }

        std::vector<unsigned> bitmasks;
        for (unsigned i = 0; i < 32; i++)
        {
            bitmasks.push_back(powf(2, 31 - i));
        }

        SDL_Rect block;
        block.w = 20;
        block.h = 20;
        SDL_FillRect(display, &display->clip_rect, 0);
        for (unsigned y = 0; y < packed.size(); y++)
        {
            for (unsigned x = 0; x < bitmasks.size(); x++)
            {
                if (packed[y] & bitmasks[x])
                {
                    block.x = x * block.w;
                    block.y = y * block.h;
                    SDL_FillRect(display, &block, SDL_MapRGB(display->format, 255, 255, 255));
                }
            }
        }

    }
    SDL_Event sdlevent;
    bool running = true;
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
                        default:break;
                    }
                }
            }
        }
        if (!viewmode)
        {
            DrawSurface(img,display,0,0,0,0,img->w,img->h);
        }
        SDL_Flip(display);
        SDL_Delay(100);
    }

    if (!viewmode)
    {
        PackImage(img, argv[2]);
        if (img != NULL)
        {
            SDL_FreeSurface(img);
        }
    }



    // cleanup
    SDL_Quit();
    return 0;
}
