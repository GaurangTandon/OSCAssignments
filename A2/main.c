// this file is the core for interacting with the user

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "commands.h"
#include "directory.h"
#include "history.h"
#include "pinfo.h"
#include "prompt.h"
#include "takeInput.h"

void sigintHandlerC(int sig_num) {
    int process = processpid;
    if (process > 0 && process != getpid()) {
        raise(SIGINT);
    }
}

void sigintHandlerZ(int sig_num) {
    int process = processpid;
    if (process > 0 && process != getpid()) {
        raise(SIGTSTP);
    }
}

void initSetup() {
    initDirSetup(1);
    historySetup();

    signal(SIGINT, sigintHandlerC);
    signal(SIGTSTP, sigintHandlerZ);
}

void inputter() {
    int commandsCount = 0;
    processpid = 0;
    char** commands = takeInput(&commandsCount);
    checkPending();
    for (int i = 0; i < commandsCount; i++) {
        execCommand(commands[i]);
    }
    fflush(stdout);
    if (commandsCount == 0) {
        printf("No command entered\n");
        fflush(stdout);
    }
}

int main() {
    initSetup();
    printWelcomeScreen();

    while (1) {
        inputter();
    }
}