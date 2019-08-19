char** tokenizeCommands(char* command, int len) {
    // assume 100 commands will input at once
    char* commands[100];

    // exit parsing as soon as # is seen

    // whether single or double quotes are on
    int singleOn = 0, doubleOn = 0;

    for (int i = 0; i < len; i++) {
        char c = command[i];

        if ((c == '#' && !singleOn && !doubleOn) ||
            (c == ';' && !singleOn && !doubleOn)) {
            break;
        } else if (c == '\'') {
            singleOn = !singleOn;
        } else if (c == '\"') {
            doubleOn = !doubleOn;
        } else {
        }
    }
}