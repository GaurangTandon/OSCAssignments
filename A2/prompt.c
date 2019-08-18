#include "prompt.h"
#include <stdio.h>
#include "colors.h"
#include "directory.h"
#include "sysData.h"

void printWelcomeScreen() {
    printf("Welcome to " KGRN "GOSH!" KNRM " aka GOrang's SHell :)\n");
}

// we cannot use getcwd since the directory from which
// our program is executed is supposed to be the home directory
void printPWD() {
    if (currDirectory >= 1) {
        printf("~");
        for (int i = 2; i <= currDirectory; i++) {
            printf("/%s", currDirectories[i]);
        }
    } else {
        printf("/");
        if (currDirectory == 0) {
            printf("home");
        }
    }
}

void printPrompt() {
    printf("%s@", getUser());
    printMachine();
    printf(":");
    printPWD();
    printf("$ ");
}