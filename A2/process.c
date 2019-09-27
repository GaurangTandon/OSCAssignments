#include "process.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "commands.h"
#include "pinfo.h"
#include "stringers.h"
#define _GNU_SOURCE

int execProcess(char* cmd, char** args, int isBackgroundJob) {
    int child = fork();

    if (child == -1) {
        perror("Could not fork child");
        return -1;
    } else if (child == 0) {
        // pid=0, gid=0
        setpgid(0, 0);

        /**
         * echo and probably a lot of other shell builtins are really messed up
         * here because i am sending each space separated part as an argument
         * whereas they expect it as one single argument
         * Using execvpe since processes must inherit environment variables now
         */
        if (execvpe(cmd, args, __environ) < 0) {
            perror("Couldn't execute command");
        }

        exit(0);
    } else {
        // wait for child to complete
        if (!isBackgroundJob) {
            foregroundProcId = child;

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            setpgid(child, 0);
            tcsetpgrp(0, __getpgid(child));
            int st;
            waitpid(child, &st, WUNTRACED);

            tcsetpgrp(0, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            // if we get ctrl z signal
            if (WIFSTOPPED(st)) {
                pendingIDs[pendingCount] = child;
                pendingNames[pendingCount] = cmd;
                pendingCount++;
            }
            foregroundProcId = 0;
        } else {
            setpgid(child, 0);
            tcsetpgrp(0, getpgrp());
        }
    }
    return (isBackgroundJob ? child : -1);
}

void interruptPrint(int doNotPrintFirstLine) {
    int fd = open("/proc/interrupts", O_RDONLY);
    char* value = (char*)malloc(10000);
    read(fd, value, 10000);

    char* line = strtok(value, "\n");
    if (!doNotPrintFirstLine)
        printf("%s\n", trim(line));

    line = strtok(NULL, "\n");
    line = strtok(NULL, "\n");

    int i = 0;
    while (line[i]) {
        if (line[i] == 'I') {
            line[i] = 0;
            break;
        }
        i++;
    }

    printf("%s\n", trim(line + 5));
    fflush(stdout);

    close(fd);
}

void dirtyMemPrint() {
    int fd = open("/proc/meminfo", O_RDONLY);
    char* value = (char*)malloc(10000);
    read(fd, value, 10000);
    char* ptr = strtok(value, "\n");
    for (int i = 0; i < 16; i++) {
        ptr = strtok(NULL, "\n");
    }
    printf("%s\n", trim(ptr + 6));
    fflush(stdout);

    close(fd);
}