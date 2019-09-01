#include <fcntl.h>
#include <math.h>
#include <signal.h>
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
#include "prompt.h"
#include "stringers.h"

#define stater struct termios

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
                printf("with status %d", ec);
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

void rawModeOn() {
    stater termas;
    tcgetattr(0, &termas);
    termas.c_lflag &= ~(ICANON | ECHO);  // Disable echo as well
    tcsetattr(0, TCSANOW, &termas);
}

void rawModeGone() {
    stater termas;
    tcgetattr(0, &termas);
    termas.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &termas);
}

int keyboardWasPressed() {
    int bytesarewaitingforme;
    rawModeOn();
    ioctl(0, FIONREAD, &bytesarewaitingforme);
    int ans = bytesarewaitingforme > 0;
    rawModeGone();
    return ans;
}

void execCommand(char* command) {
    char* cmd2 = (char*)malloc(strlen(command));
    memcpy(cmd2, command, strlen(command) + 1);

    // if command is just a combo of up arrows and down arrows
    if (command[0] == 27) {
        int offset = 0;
        for (int i = 2; i < (int)strlen(command); i += 3) {
            offset += (command[i] == 'A' ? 1 : -1);
        }

        // offset > 0 denotes how many history items we have to go up into
        if (offset < 0 || storedCount < offset) {
            printf("Command doesn't exist");
        } else {
            command = commandHistory[storedCount - offset];
            memcpy(cmd2, command, strlen(command) + 1);
            printPrompt();
            printf("%s\n", cmd2);
            execCommand(cmd2);
        }
        return;
    }

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
    } else if (!strcmp(cmd, "quit")) {
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
        int displayCount = 10;

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
                int msec = 0, iterations = 0;
                interval *= 1000;
                clock_t before = clock();

                if (printValue)
                    dirtyMemPrint();
                else
                    interruptPrint(c++);

                while (1) {
                    clock_t difference = clock() - before;
                    msec = difference * 1000 / CLOCKS_PER_SEC;
                    iterations++;

                    if (keyboardWasPressed()) {
                        char c = getchar();
                        if (c == 'q') {
                            printf("\n");
                            fflush(stdout);
                            break;
                        }
                    }
                    if (msec >= interval) {
                        before = clock();
                        if (printValue)
                            dirtyMemPrint();
                        else
                            interruptPrint(c++);
                    }
                }
            }
        }

    } else if (!strcmp(cmd, "jobs")) {
        for (int i = 0; i < pendingCount; i++) {
            char* pid = (char*)malloc(100);
            snprintf(pid, 100, "%d", pendingIDs[i]);

            // check status of process here
            char* status = getProcStatusString(pid);

            printf("[%d] %s %s [%s]\n", i + 1, status, pendingNames[i], pid);
        }
    } else if (!strcmp(cmd, "kjob")) {
        if (argCount < 3) {
            printf("Error: usage kjob <jobNumber> <signalNumber>\n");
            return;
        }
        union sigval;

        int pid = atoi(args[1]), signalNumber = atoi(args[2]);

        kill(pid, signalNumber);
    } else if (!strcmp(cmd, "setenv")) {
        if (argCount < 2 || argCount > 3) {
            printf("Invalid syntax! Should be: setenv ​ var [value]\n");
            return;
        }

        char* var = args[1];
        char* value = "";

        if (argCount > 2) {
            value = (char*)malloc(1000);
            strcat(value, args[2]);
        }

        strcat(var, "=");
        strcat(var, value);
        putenv(var);
    } else if (!strcmp(cmd, "unsetenv")) {
        if (argCount != 2) {
            printf("Invalid syntax! Should be: unsetenv​ var\n");
            return;
        }

        char* var = args[1];
        unsetenv(var);
    } else if (!strcmp(cmd, "getenv")) {
        if (argCount != 2) {
            printf("Invalid syntax! Should be: getenv ​var\n");
            return;
        }

        char *var = args[1], *ans = getenv(var);
        if (ans)
            printf("%s\n", ans);
        else
            printf("var %s not found\n", var);
    } else {
        int idOfChild = execProcess(cmd, args, isBackgroundJob);

        if (idOfChild != -1) {
            pendingNames[pendingCount] = cmd2;
            pendingIDs[pendingCount] = idOfChild;
            pendingCount++;
        }
    }
}