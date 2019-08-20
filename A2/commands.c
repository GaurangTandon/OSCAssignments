#include <stdio.h>
#include <stdlib.h>
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
