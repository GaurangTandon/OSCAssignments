#include "directory.h"
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <math.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define amax(x, y) \
    if (x < y)     \
        x = y;

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
// isCommand = true matlab user execs pwd command
// isCommand = false matlab user is viewing shell prompt
void printPWD(int isCommand) {
    if (currDirectoryPathLen >= homeDirPathLen) {
        if (!isCommand)
            printf("~");
        else
            printf("/home/%s", getUser());
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

static int myCompare(const struct dirent** a, const struct dirent** b) {
    const struct dirent *a2 = *a, *b2 = *b;
    const char *aa = a2->d_name, *bb = b2->d_name;

    int i = 0, j = 0;
    if (*aa == '.')
        i++;
    if (*bb == '.')
        j++;

    return strcasecmp(aa + i, bb + j);
}

static int myFilter(const struct dirent* a) {
    const char* aa = a->d_name;

    return strlen(aa);
}

void printnum(int num, int len) {
    char str[len * sizeof(char) + 1];
    sprintf(str, "%d", num);
    printf("%*s", len, str);
}

long int totalSize;

void longListPrint(const char* allFiles[], int len, int showHidden) {
    int maxuname = 0, maxgname = 0, maxsize = 0, maxlink = 0;

    for (int i = 0; i < len; i++) {
        struct stat s;
        const char* file = allFiles[i];
        if (strlen(file) == 0)
            continue;
        int ret = stat(file, &s);
        if (ret < 0) {
            perror(file);
            return;
        }
        char *usrname = getpwuid(getuid())->pw_name,
             *groupname = getgrgid(getgid())->gr_name;
        int sz = s.st_size;
        amax(maxuname, (int)strlen(usrname));
        amax(maxgname, (int)strlen(groupname));
        int digs = log10(sz) + 1;
        amax(maxsize, digs);
        int digs2 = log10(s.st_nlink) + 1;
        amax(maxlink, digs2);
    }

    printf("total %ld\n", totalSize);

    for (int i = 0; i < len; i++) {
        struct stat s;
        const char* file = allFiles[i];
        if (strlen(file) == 0)
            continue;
        if (!showHidden && *file == '.')
            continue;

        int ret = stat(file, &s);
        if (ret < 0) {
            perror(file);
            return;
        }

        if (S_ISDIR(s.st_mode)) {
            printf("d");
        } else {
            printf("-");
        }

        int perms[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                        S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

        for (int i = 0; i < 9; i++) {
            if (s.st_mode & perms[i]) {
                if (i % 3 == 0) {
                    printf("r");
                } else if (i % 3 == 1) {
                    printf("w");
                } else
                    printf("x");
            } else {
                printf("-");
            }
        }

        printf(" ");
        printnum(s.st_nlink, maxlink);

        char *usrname = getpwuid(getuid())->pw_name,
             *groupname = getgrgid(getgid())->gr_name;
        printf(" %.*s %.*s ", maxuname, usrname, maxgname, groupname);
        printnum(s.st_size, maxsize);

        char* tt = ctime(&s.st_mtime);
        char tt2[15];
        memcpy(tt2, tt + 4, 12);
        tt2[13] = 0;

        printf(" %.*s", 15, tt2);
        printf(" %s\n", file);
    }
}

void ls(char* directory, int showHidden, int longListing) {
    const char* output[1000];
    int len = 0;

    struct dirent** namelist;
    int n;

    n = scandir(directory, &namelist, myFilter, myCompare);
    if (n == -1) {
        perror("scandir");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        output[i] = namelist[i]->d_name;
    }

    len = n;
    struct stat s;
    stat(directory, &s);
    totalSize = s.st_size;

    // see https://stackoverflow.com/a/23970992
    chdir(directory);

    if (longListing) {
        longListPrint(output, len, showHidden);
    } else {
        for (int i = 0; i < len; i++) {
            if (!showHidden && output[i][0] == '.')
                continue;
            printf("%s\n", output[i]);
        }
    }

    chdir(currDirString);
}

void changeDirectory(char* newDir) {
    int ret = chdir(newDir);

    if (ret != 0) {
        perror("Error changing directory");
    } else {
        initDirSetup(0);
    }
}