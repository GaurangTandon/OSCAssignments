#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "history.h"
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
        char* x = (char*)malloc(1000);
        memcpy(x, s, strlen(s));
        // do not store up arrow stuff in history
        if (x[0] != 27)
            addNewCommand(x);
        return tokenizeCommands(s, commandsCount);
    }

    return NULL;
}