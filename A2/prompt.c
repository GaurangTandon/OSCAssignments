#include "prompt.h"
#include <stdio.h>
#include "colors.h"
#include "directory.h"
#include "sysData.h"

void printWelcomeScreen() {
    printf("Welcome to " KGRN "GOSH!" KNRM " aka GOrang's SHell :)\n");
    fflush(stdout);
}

void printPrompt() {
    printf("%s@", getUser());
    printMachine();
    printf(":");
    printPWD(0);
    printf("$ ");
    fflush(stdout);
}