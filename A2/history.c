#include "history.h"
#include <stdio.h>
#include <string.h>

#define max(x, y) (((x) >= (y)) ? (x) : (y))
#define maxStored 20

char* commandHistory[maxStored];
int storedCount = 0;
void addNewCommand(char* cmd) {
    if (storedCount > 0 && strcmp(commandHistory[storedCount - 1], cmd) == 0) {
        return;
    }

    if (storedCount < maxStored) {
        commandHistory[storedCount++] = cmd;
    } else {
        for (int i = 0; i < maxStored - 1; i++) {
            commandHistory[i] = commandHistory[i + 1];
        }
        commandHistory[maxStored - 1] = cmd;
    }
}

void printHistory(int n) {
    for (int i = storedCount - 1; i >= max(storedCount - n, 0); i--) {
        printf("%s\n", commandHistory[i]);
    }
}