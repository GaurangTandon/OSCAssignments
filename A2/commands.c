#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "directory.h"
#include "history.h"
#include "print.h"
#include "process.h"
#include "stringers.h"

char** tokenizeCommands(char* allCommandsString, int* commandsCountRef) {
    char** commands = (char**)malloc(100);
    for (int i = 0; i < 100; i++) {
        commands[i] = (char*)malloc(100);
    }
    char* delim = ";";

    int commandsCount = 0;
    char* ptr = strtok(allCommandsString, delim);
    while (ptr) {
        commands[commandsCount++] = ptr;
        ptr = strtok(NULL, delim);
    }

    *commandsCountRef = commandsCount;

    return commands;
}

void execCommand(char* command) {
    char* cmd2 = (char*)malloc(strlen(command));
    memcpy(cmd2, command, strlen(command) + 1);
    addNewCommand(cmd2);

    // first parse main command and all its args
    char* delim = " ";
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
        printPWD(1);
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
        int pid = getpid();
        int len = log10(pid) + 1;
        char* str = (char*)malloc(len * sizeof(char));
        sprintf(str, "%d", pid);

        int isSelf = 1;

        if (argCount == 2) {
            str = args[1];
            isSelf = 0;
        }
        printf("PID:\t\t\t%s\n", str);

        char path[100] = "/proc/";
        strcat(path, str);
        strcat(path, "/stat");

        int statfile = open(path, O_RDONLY);
        char* buf = (char*)malloc(1000);
        read(statfile, buf, 10000);
        char* ptr = strtok(buf, " ");
        ptr = strtok(NULL, " ");
        ptr = strtok(NULL, " ");

        printf("Process status:\t\t%s\n", ptr);
        close(statfile);

        strcat(path, "m");
        statfile = open(path, O_RDONLY);
        buf = (char*)malloc(1000);
        read(statfile, buf, 10000);
        ptr = strtok(buf, " ");
        printf("Memory:\t\t\t%s\n", ptr);
        close(statfile);

        char path2[100] = "/proc/";
        strcat(path2, str);
        strcat(path2, "/exe");
        buf = (char*)malloc(1000);
        readlink(path2, buf, 1000);

        if (isSelf) {
            printf("Executable path: \t");
            printPWD(0);
            printf("/a.out");
        } else
            printf("Executable path: \t%s", buf);
        printf("\n");
        fflush(stdout);
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
                while (1) {
                    if (printValue)
                        dirtyMemPrint();
                    else
                        interruptPrint(c++);
                    printf("%c", getchar());
                    sleep(interval);
                }
            }
        }
    } else {
        execProcess(cmd, args, isBackgroundJob);
    }
}