#include "history.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "directory.h"
#include "stringers.h"

#define max(x, y) (((x) >= (y)) ? (x) : (y))
#define maxStored 20

char* commandHistory[maxStored];
int storedCount = 0;
char* fileName;

void retrieveStored() {
    int fd = open(fileName, O_CREAT | O_RDONLY, S_IRWXU);
    // nevar use square brackets
    char* buf = (char*)malloc(1000);
    read(fd, buf, 1000);

    if (fd < 0) {
        perror("Couldn't open history file while reading");
        return;
    }

    storedCount = 0;

    char* ptr = strtok(buf, "\n");
    while (ptr) {
        ptr = trim(ptr);
        if (strlen(ptr) != 0) {
            commandHistory[storedCount++] = ptr;
        }
        ptr = strtok(NULL, "\n");
    }

    close(fd);
}

void writeStored() {
    int fd = open(fileName, O_TRUNC | O_WRONLY, S_IRWXU);

    if (fd < 0) {
        perror("Couldn't open history file while writing");
        return;
    }

    for (int i = 0; i < storedCount; i++) {
        write(fd, commandHistory[i], strlen(commandHistory[i]));
        write(fd, "\n", 1);
    }

    close(fd);
}

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
    writeStored();
}

void printHistory(int n) {
    int lim = max(storedCount - n, 0);
    for (int i = lim; i <= storedCount - 1; i++) {
        printf("%s", commandHistory[i]);
        printf("\n");
    }
}

void historySetup() {
    fileName = (char*)malloc(1000);
    strcat(fileName, expectedHomeDir);
    strcat(fileName, "/history.txt");
    retrieveStored();
}