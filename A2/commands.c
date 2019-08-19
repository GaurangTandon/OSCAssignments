char** tokenizeCommands(char* allCommandsString, int len,
                        int* commandsCountRef) {
    // assume 100 commands will input at once
    char* commands[100];
    char* latestCommand[100];

    // exit parsing as soon as # is seen

    // whether single or double quotes are on
    int singleOn = 0, doubleOn = 0, commandsCount = 0, latestCommandLen = 0;

    for (int i = 0; i < len; i++) {
        char c = allCommandsString[i];

        if ((c == '#' && !singleOn && !doubleOn) ||
            (c == ';' && !singleOn && !doubleOn)) {
            latestCommand[latestCommandLen++] = 0;
            commands[commandsCount++] = latestCommand;
            latestCommand[0] = 0;
            latestCommandLen = 0;
            break;
        } else if (c == '\'') {
            singleOn = !singleOn;
        } else if (c == '\"') {
            doubleOn = !doubleOn;
        } else {
            latestCommand[latestCommandLen++] = c;
        }
    }

    if (latestCommandLen > 0) {
        latestCommand[latestCommandLen++] = 0;
        commands[commandsCount++] = latestCommand;
    }

    *commandsCountRef = commandsCount;
    return commands;
}
