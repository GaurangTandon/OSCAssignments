// this file is the core for interacting with the user

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "commands.h"
#include "directory.h"
#include "history.h"
#include "pinfo.h"
#include "prompt.h"
#include "takeInput.h"

void sigintHandlerC() {
    if (foregroundProcId > 0 && foregroundProcId != getpid()) {
        raise(SIGINT);
    }
}

void sigintHandlerZ() {
    if (foregroundProcId > 0 && foregroundProcId != getpid()) {
        raise(SIGTSTP);
    }
}

void inputter() {
    int commandsCount = 0;
    foregroundProcId = 0;
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
void childTermination() {
    int st;
    int pid = waitpid(-1, &st, WNOHANG);

    if (pid <= 0)
        return;

    char* naam;
    // find this pid in the jobs array and eliminate it
    for (int i = 0; i < pendingCount; i++) {
        if (pendingIDs[i] == pid) {
            pendingIDs[i] = 0;
            naam = pendingNames[i];
        }
    }

    printf("JOBS: Process %s (%d) exited ", naam, pid);

    if (WIFEXITED(st) && WEXITSTATUS(st) == EXIT_SUCCESS) {
        printf("normally.");
    } else {
        printf("with nzec %d.", WEXITSTATUS(st));
    }
    printf("\n");
    fflush(stdout);
    // print prompt again, do not wait for input as inputter is already there
    // doing this
    printPrompt();

    return;
}

void initSetup() {
    initDirSetup(1);
    historySetup();

    signal(SIGCHLD, childTermination);
    signal(SIGINT, sigintHandlerC);
    signal(SIGTSTP, sigintHandlerZ);
}

int main() {
    initSetup();
    printWelcomeScreen();

    while (1) {
        inputter();
    }
}