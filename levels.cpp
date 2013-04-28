#include "levels.h"
#include "switch.h"
#include "laser.h"
#include <iostream>

unsigned LEVEL_COUNT = 0;

void InitLevels()
{
    FILE* fp = fopen("levels.txt", "r");
    if (fp != NULL)
    {
        char dummy[24];
        fscanf(fp, "%s %d\n", dummy, &LEVEL_COUNT);
        fclose(fp);
    }
}

bool LoadLevelData(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return false;
    }

    char key;
    unsigned x, y, cfg, switchid, laserid, path;

    while(true)
    {
        fscanf(fp, "%c ", &key);
        if (key == 'l')
        {
            fscanf(fp, "%u %u %u\n", &x, &y, &cfg);
            x *= 128;
            x += 64;
            y *= 128;
            y += 64;
            Laser::Add(x, y, cfg);
            std::cerr << "adding laser:" << x << " " << y << " " << cfg << "\n";
        }
        else if (key == 's')
        {
            fscanf(fp, "%u %u\n", &x, &y);
            Switch sw;
            sw.posx = SwitchX(x);
            sw.posy = SwitchY(y);
            Switch::Add(sw);
            std::cerr << "adding switch:" << x << " " << y << " " << cfg << "\n";
        }
        else if (key == 'c')
        {
            fscanf(fp, "%u %u %u\n", &switchid, &laserid, &path);
            Switch& swr = Switch::switches[switchid];
            swr.AddConnection(laserid, path);
            std::cerr << "adding switch connection:" << switchid << " " << laserid << " " << path << "\n";
        }
        else
        {
            std::cerr << "unknown key:" << key << "\n";
        }
        if (feof(fp))
        {
            break;
        }
    }

    fclose(fp);
    return true;
}

bool LoadLevel(unsigned level)
{
    if (level >= LEVEL_COUNT)
    {
        return false;
    }

    Laser::lasers.clear();
    Switch::switches.clear();
    char filename[2048];
    sprintf(filename, "levels/level%d.txt", 1 + level);
    if (!LoadLevelData(filename))
    {
        std::cerr << "failed to load level:" << filename << "\n";
        return false;
    }
    return true;
}

// helpers
