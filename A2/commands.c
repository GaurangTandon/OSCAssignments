#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include "directory.h"
#include "history.h"
#include "pinfo.h"
#include "print.h"
#include "process.h"
#include "stringers.h"

char* pendingNames[100];
int pendingIDs[100];
int pendingCount = 0;

void checkPending() {
    int statusThings[pendingCount];

    for (int i = 0; i < pendingCount; i++) {
        statusThings[i] = 0;
        int st;
        int ret = waitpid(pendingIDs[i], &st, WNOHANG);
        if (ret == -1) {
            char buf[100];
            sprintf(buf, "Error checking status %d", pendingIDs[i]);
            perror(buf);
            statusThings[i] = 1;
            continue;
        } else if (ret == 0) {
            continue;
        } else if (WIFEXITED(st)) {
            int ec = WEXITSTATUS(st);
            printf("Process %s with id %d exited ", pendingNames[i],
                   pendingIDs[i]);
            if (ec == 0) {
                printf("normally");
            } else {
                printf("with status %d", st);
            }

            printf("\n");

            statusThings[i] = 1;
        }
    }
    fflush(stdout);
    int o = pendingCount;
    pendingCount = 0;

    for (int i = 0, j = 0; i < o; i++) {
        if (!statusThings[i]) {
            pendingIDs[j] = pendingIDs[i];
            pendingNames[j] = pendingNames[i];
            j++;
            pendingCount++;
        }
    }
}

char** tokenizeCommands(char* allCommandsString, int* commandsCountRef) {
    char** commands = (char**)malloc(100);
    for (int i = 0; i < 100; i++) {
        commands[i] = (char*)malloc(100);
    }
    char* delim = ";";

    int commandsCount = 0;
    char* ptr = strtok(allCommandsString, delim);
    while (ptr) {
        char* actual = trim(ptr);
        if (strlen(actual) != 0) {
            commands[commandsCount++] = actual;
        }
        ptr = strtok(NULL, delim);
    }

    *commandsCountRef = commandsCount;

    return commands;
}

void enable_raw_mode() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);  // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

int _kbhit() {
    int byteswaiting;
    enable_raw_mode();
    ioctl(0, FIONREAD, &byteswaiting);
    int ans = byteswaiting > 0;
    disable_raw_mode();
    return ans;
}

void execCommand(char* command) {
    char* cmd2 = (char*)malloc(strlen(command));
    memcpy(cmd2, command, strlen(command) + 1);
    addNewCommand(cmd2);

    // first parse main command and all its args
    char* delim = "\t ";
    char** args = (char**)malloc(100);
    int argCount = 0;
    int firstCall = 1;

    while (1) {
        char* arg;
        if (firstCall) {
            arg = strtok(command, delim);
            firstCall = 0;
        } else {
            arg = strtok(NULL, delim);
        }
        if (!arg)
            break;
        args[argCount++] = arg;
    }

    for (int i = 0; i < argCount; i++) {
        args[i] = trim(args[i]);
    }

    // actual args begin from 1
    char* cmd = args[0];

    int isBackgroundJob = 0;
    for (int i = 0; i < argCount; i++) {
        if (strcmp(args[i], "&") == 0) {
            isBackgroundJob = 1;
            break;
        }
    }

    if (!strcmp(cmd, "ls")) {
        char* dir = ".";
        int hiddenShow = 0, longlist = 0;

        for (int i = 1; i < argCount; i++) {
            char* arg = args[i];
            if (strlen(arg) == 0)
                continue;
            if (arg[0] == '-') {
                for (int j = 1; j < (int)strlen(arg); j++) {
                    switch (arg[j]) {
                        case 'a':
                            hiddenShow = 1;
                            break;
                        case 'l':
                            longlist = 1;
                            break;
                    }
                }
            } else if (strcmp("&", arg)) {
                dir = arg;
            }
        }
        ls(dir, hiddenShow, longlist);
    } else if (!strcmp(cmd, "exit")) {
        exit(0);
    } else if (!strcmp(cmd, "cd")) {
        char* target = "~";
        if (argCount > 1)
            target = args[1];
        changeDirectory(target);
    } else if (!strcmp(cmd, "pwd")) {
        printPWD(1, 1);
    } else if (!strcmp(cmd, "echo")) {
        char* finalArg = (char*)malloc(1000);
        int len = 0;
        for (int i = 1; i < argCount; i++) {
            char* arg = args[i];
            for (int j = 0; j < (int)strlen(arg); j++) {
                finalArg[len++] = arg[j];
            }
            finalArg[len++] = ' ';
        }
        finalArg[len] = 0;
        // join all args by spaces and then print :(
        echo(finalArg, len);
    } else if (!strcmp(cmd, "pinfo")) {
        printPinfo(argCount, args);
    } else if (!strcmp(cmd, "history")) {
        int displayCount = 3;

        if (argCount == 2) {
            displayCount = atoi(args[1]);
        }

        printHistory(displayCount);
    } else if (!strcmp(cmd, "nightswatch")) {
        int takeInterval = 0, interval = -1;

        for (int i = 1; i < argCount; i++) {
            if (!strcmp(args[i], "-n")) {
                takeInterval = 1;
                continue;
            }
            if (takeInterval) {
                interval = atoi(args[i]);
                takeInterval = 0;
            } else if (interval == -1) {
                printf("Did not specify interval");
            } else {
                int printValue = 0;
                if (!strcmp(args[i], "dirty")) {
                    printValue = 1;
                }
                int c = 0;
                int msec = 0, iterations = 0, interval = 1000;
                clock_t before = clock();

                while (1) {
                    clock_t difference = clock() - before;
                    msec = difference * 1000 / CLOCKS_PER_SEC;
                    iterations++;

                    if (msec >= interval) {
                        before = clock();

                        if (_kbhit()) {
                            char c = getchar();
                            if (c == 'q') {
                                printf("\n");
                                fflush(stdout);
                                break;
                            }
                        }
                        if (printValue)
                            dirtyMemPrint();
                        else
                            interruptPrint(c++);
                    }
                }
            }
        }
    } else {
        int idOfChild = execProcess(cmd, args, isBackgroundJob);

        if (idOfChild != -1) {
            pendingNames[pendingCount] = cmd;
            pendingIDs[pendingCount] = idOfChild;
            pendingCount++;
        }
    }
}