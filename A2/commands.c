#include <stdio.h>
#include <stdlib.h>

void tokenizeCommands(char* allCommandsString, int len, int* commandsCountRef,
                      char** commands) {
    char latestCommand[100];

    // whether single or double quotes are on
    int singleOn = 0, doubleOn = 0, commandsCount = 0, latestCommandLen = 0;

    for (int i = 0; i < len; i++) {
        char c = allCommandsString[i];

        // exit parsing as soon as #, ; is seen
        if ((c == '#' && !singleOn && !doubleOn) ||
            (c == ';' && !singleOn && !doubleOn)) {
            latestCommand[latestCommandLen++] = 0;
            commands[commandsCount++] = latestCommand;
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
        commands[commandsCount++] = latestCommand;
    }

    *commandsCountRef = commandsCount;
}
