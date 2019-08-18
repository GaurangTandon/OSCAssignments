#include "directory.h"
#include <stdio.h>
#include <string.h>

char* currDirectories[100] = {
    "home",
};

int currDirectory = 0;
char currDirString[1000];

// we cannot use getcwd since the directory from which
// our program is executed is supposed to be the home directory
void updatePWD() {
    currDirString[0] = 0;
    if (currDirectory >= 1) {
        strcat(currDirString, "~");
        for (int i = 2; i <= currDirectory; i++) {
            strcat(currDirString, "/");
            strcat(currDirString, currDirectories[i]);
        }
    } else {
        strcat(currDirString, "/");
        if (currDirectory == 0) {
            strcat(currDirString, "home");
        }
    }
}

void initDirSetup() {
    currDirectories[1] = getUser();
    currDirectory++;
    updatePWD();
}

int changeDirectory(char* newDir) {
}