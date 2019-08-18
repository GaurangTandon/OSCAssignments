#include "directory.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// i am trying to maintain two directory paths
// one path is the path to the file (constant) - so i can resolve home directory
// correctly. The other is the path the user is fiddling with currently.
// curr dir string helps us quickly pass a string to stat and other functions

char* currDirectories[100];
int currDirectoryPathLen = 0;
char currDirString[1000];

char expectedHomeDir[1000];
char* homeDirPath[1000];
int homeDirPathLen = 0;

// we cannot use getcwd since the directory from which
// our program is executed is supposed to be the home directory
void updatePWD() {
    currDirString[0] = 0;
    for (int i = 0; i < currDirectoryPathLen; i++) {
        strcat(currDirString, "/");
        strcat(currDirString, currDirectories[i]);
    }
}

// modify currDirString so that the location of the executable is treated
// as the ~
void printPWD() {
    if (currDirectoryPathLen >= homeDirPathLen) {
        printf("~");
        for (int i = homeDirPathLen; i < currDirectoryPathLen; i++) {
            printf("/%s", currDirectories[i]);
        }
    } else {
        printf("%s", currDirString);
    }
}

void initDirSetup(int updateHome) {
    getcwd(expectedHomeDir, 1000);
    char* ptr = strtok(expectedHomeDir, "/");
    currDirectoryPathLen = 0;

    if (updateHome)
        homeDirPathLen = 0;

    while (ptr != NULL) {
        if (updateHome)
            homeDirPath[homeDirPathLen++] = ptr;
        currDirectories[currDirectoryPathLen++] = ptr;
        ptr = strtok(NULL, "/");
    }

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

void changeDirectory(char* newDir) {
    int ret = chdir(newDir);

    if (ret != 0) {
        perror("Error changing directory");
    } else {
        initDirSetup(0);
    }
}