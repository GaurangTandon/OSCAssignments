#include "prompt.h"
#include <stdio.h>
#include "colors.h"
#include "directory.h"
#include "sysData.h"
#define clear() printf("\033[H\033[J")

void printWelcomeScreen() {
    clear();
    printf("Welcome to " KGRN "GOSH!" KNRM " aka " KGRN "GO" KNRM "rang's " KGRN
           "SH" KNRM "ell :)\n");
    fflush(stdout);
}

void printPrompt() {
    printf(KGRN "%s@", getUser());
    printMachine();
    printf(":");
    printPWD(1);
    printf("$ " KNRM);
    fflush(stdout);
}