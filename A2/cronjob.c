#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#define min(x, y) ((x) <= (y)) ? (x) : (y)

void cronJob(char* cmd, int interval, int time) {
    int f = fork();
    // do it in child process
    if (f == 0) {
        int curr = 0;
        while (curr < time) {
            int sleepTime = min(time - curr, interval);
            sleep(sleepTime);

            char* cpy = (char*)calloc(1, 1000);
            strcat(cpy, cmd);
            execCommand(cpy);
            curr += sleepTime;
        }
    }
}

void cronjobParse(char** args) {
    char* cmd = (char*)calloc(1, 1000);
    int interval = -1, time = -1;

    int i = 0;
    int needs[3] = {0, 0, 0};
    while (args[i]) {
        int f = 0;
        if (!strcmp(args[i], "-c")) {
            needs[0] = 1;
            f = 1;
        }
        if (!strcmp(args[i], "-t")) {
            needs[0] = 0;
            needs[1] = 1;
            f = 1;
        }
        if (!strcmp(args[i], "-p")) {
            needs[0] = 0;
            needs[2] = 1;
            f = 1;
        }
        if (f) {
            i++;
            continue;
        }
        if (needs[0]) {
            strcat(cmd, args[i]);
            strcat(cmd, " ");
        }
        if (needs[1]) {
            interval = atoi(args[i]);
            needs[1] = 0;
        }
        if (needs[2]) {
            time = atoi(args[i]);
            needs[2] = 0;
        }
        i++;
    }

    if (!cmd[0] || interval == -1 || time == -1) {
        printf(
            "Invalid format. Syntax: cronjob -c <cmd> -t <time> -p <period>");
        return;
    }

    cronJob(cmd, interval, time);
}