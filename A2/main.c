// this file is the core for interacting with the user

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "directory.h"
#include "prompt.h"

void initSetup() {
    initDirSetup(1);
}

int main() {
    // initSetup();
    // printWelcomeScreen();
    // printPrompt();
    char* commandList = "echo a; echo 'a'; echo \"c\"; echo ';'; echo \";\";";
    int totalCmds = 0;

    char** commands =
        tokenizeCommands(commandList, strlen(commandList), &totalCmds);
    for (int i = 0; i < totalCmds; i++) {
        printf("%s\n", commands[i]);
    }
}