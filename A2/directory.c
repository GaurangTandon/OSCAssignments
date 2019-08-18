#include "directory.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char* currDirectories[100] = {
    "home",
};

int currDirectory = 0;
char currDirString[1000];

// we cannot use getcwd since the directory from which
// our program is executed is supposed to be the home directory
void updatePWD() {
    currDirString[0] = 0;
    strcat(currDirString, "/");
    for (int i = 0; i <= currDirectory; i++) {
        strcat(currDirString, "/");
        strcat(currDirString, currDirectories[i]);
    }
}

// modify currDirString so that the location of the executable is treated
// as the ~
void printPWD() {
}

void initDirSetup() {
    currDirectories[1] = getUser();
    currDirectory++;
    updatePWD();
}

char** getAllFilesInDir() {
    DIR* d;
    struct dirent* dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
}

int changeDirectory(char* newDir) {
    DIR* d;
    struct dirent* dir;
    d = opendir(currDirString);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            struct stat s;
            stat(dir->d_name, &s);
            if (S_ISDIR(s.st_mode) && strcmp(dir->d_name, newDir) == 0) {
                currDirectories[++currDirectory] = newDir;
                return 0;
            }
        }
        closedir(d);
    }

    return -1;
}