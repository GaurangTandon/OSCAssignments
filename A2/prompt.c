#include "prompt.h"
#include <stdio.h>
#include "colors.h"
#include "directory.h"
#include "sysData.h"

void printWelcomeScreen() {
    printf("Welcome to " KGRN "GOSH!" KNRM " aka " KGRN "GO" KNRM "rang's " KGRN
           "SH" KNRM "ell :)\n");
    fflush(stdout);
}

void printPrompt() {
    printf(KGRN "%s@", getUser());
    printMachine();
    printf(":");
    printPWD(0);
    printf("$ " KNRM);
    fflush(stdout);
}