// this file is the core for interacting with the user

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "directory.h"
#include "prompt.h"

void initSetup() {
    initDirSetup();
}

int main() {
    initSetup();
    printWelcomeScreen();
    printPrompt();
}