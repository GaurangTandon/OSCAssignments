#include "directory.h"

char* currDirectories[100] = {
    "home",
};

int currDirectory = 0;

void initDirSetup() {
    currDirectories[1] = getUser();
    currDirectory++;
}