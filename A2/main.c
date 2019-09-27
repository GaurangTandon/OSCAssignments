// this file is the core for interacting with the user

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "directory.h"
#include "history.h"
#include "pinfo.h"
#include "prompt.h"
#include "takeInput.h"

void sigintHandlerC(int sig_num) {
    int process = processpid;
    if (process > 0) {
        kill(process, sig_num);
        printf("\n");
        fflush(stdout);
    } else {
        printf("\n");
        takeInput();
        fflush(stdout);
    }
}

void sigintHandlerZ(int sig_num) {
    int process = processpid;

    if (process > 0) {
        kill(process, sig_num);
        char* x = getProcName(process);
        pendingNames[pendingCount] = x;
        pendingIDs[pendingCount] = process;
        pendingCount++;
        printf("\n");
        fflush(stdout);
    } else {
        printf("\n");
        takeInput();
        fflush(stdout);
    }
}

void initSetup() {
    initDirSetup(1);
    historySetup();

    signal(SIGINT, sigintHandlerC);
    signal(SIGTSTP, sigintHandlerZ);
}

int main() {
    initSetup();
    printWelcomeScreen();

    while (1) {
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
}