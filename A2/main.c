// this file is the core for interacting with the user

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "directory.h"
#include "prompt.h"
#include "takeInput.h"

void initSetup() {
    initDirSetup(1);
}

int main() {
    initSetup();
    printWelcomeScreen();

    while (1) {
        int commandsCount = 0;
        char** commands = takeInput(&commandsCount);
        for (int i = 0; i < commandsCount; i++) {
            execCommand(commands[i]);
        }
        fflush(stdout);
        if (commandsCount == 0) {
            printf("No command entered\n");
            fflush(stdout);
        }
    }
}