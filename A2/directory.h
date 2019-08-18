#pragma once

#include "sysData.h"

extern char* currDirectories[100];
extern char currDirString[1000];
// 0 indicates root
extern int currDirectoryPathLen;

void initDirSetup();
// return -1 on error; 0 on success
int changeDirectory(char* newDir);