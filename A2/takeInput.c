#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "prompt.h"

char** takeInput(int* commandsCount) {
    printPrompt();
    char* s = (char*)calloc(1, 100);
    int len = 0;
    char c;
    while ((c = getchar()) != '\n') {
        s[len++] = c;
    }

    if (len != 0) {
        return tokenizeCommands(s, strlen(s), commandsCount);
    }

    return NULL;
}