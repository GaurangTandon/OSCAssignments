#pragma once

#include "sysData.h"

char* currDirectories[100] = {
    "home",
};
// -1 indicates root
int currDirectory = 0;

void initDirSetup();