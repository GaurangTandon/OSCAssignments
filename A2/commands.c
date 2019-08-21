#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "directory.h"
#include "print.h"
#include "stringers.h"

char** tokenizeCommands(char* allCommandsString, int len,
                        int* commandsCountRef) {
    char** commands = (char**)malloc(100);
    for (int i = 0; i < 100; i++) {
        commands[i] = (char*)malloc(100);
    }
    char* latestCommand = (char*)malloc(100);

    // whether single or double quotes are on
    int singleOn = 0, doubleOn = 0, commandsCount = 0, latestCommandLen = 0;

    for (int i = 0; i < len; i++) {
        char c = allCommandsString[i];

        // exit parsing as soon as #, ; is seen
        if ((c == '#' && !singleOn && !doubleOn) ||
            (c == ';' && !singleOn && !doubleOn)) {
            latestCommand[latestCommandLen++] = 0;
            commands[commandsCount++] = trim(latestCommand);
            latestCommand = (char*)malloc(100);
            latestCommand[0] = 0;
            latestCommandLen = 0;
            continue;
        } else if (c == '\'') {
            singleOn = !singleOn;
        } else if (c == '\"') {
            doubleOn = !doubleOn;
        }
        latestCommand[latestCommandLen++] = c;
    }

    if (latestCommandLen > 0) {
        latestCommand[latestCommandLen++] = 0;
        commands[commandsCount++] = trim(latestCommand);
    }

    *commandsCountRef = commandsCount;

    return commands;
}

void execCommand(char* command) {
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
            } else {
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
        printPWD();
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
    } else {
        pid_t child = fork();

        if (child == -1) {
            perror("Could not fork child");
            return;
        } else if (child == 0) {
            if (execvp(cmd, args) < 0) {
                perror("Couldn't execute command: ");
            }
            // kill child process
            exit(0);
        } else {
            // wait for child to complete
            if (!isBackgroundJob)
                wait(NULL);
            return;
        }
    }
}