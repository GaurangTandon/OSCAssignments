void cronJob(char* cmd, int interval, int time) {
}

void cronjobParse(char** args) {
    char* cmd;
    int interval, time;

    int i = 0;
    int needs[3] = {0, 0, 0};
    while (args[i]) {
        if (needs[0]) {
            cmd = args[i];
            needs[0] = 0;
        }
        if (needs[1]) {
            interval = atoi(args[i]);
            needs[1] = 0;
        }
        if (needs[2]) {
            time = atoi(args[i]);
            needs[2] = 0;
        }
        if (!strcmp(args[i], "-c")) {
            needs[0] = 1;
        }
        if (!strcmp(args[i], "-t")) {
            needs[1] = 1;
        }
        if (!strcmp(args[i], "-p")) {
            needs[2] = 1;
        }
        i++;
    }

    cronJob(cmd, interval, time);
}