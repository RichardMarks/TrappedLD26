#ifndef __TRAPPEDLD26_LEVELS_H__
#define __TRAPPEDLD26_LEVELS_H__

extern unsigned LEVEL_COUNT;
extern void InitLevels();
extern bool LoadLevelData(const char* filename);
extern bool LoadLevel(unsigned level);

#endif
