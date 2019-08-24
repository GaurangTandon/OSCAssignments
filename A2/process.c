#include "process.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "stringers.h"

int execProcess(char* cmd, char** args, int isBackgroundJob) {
    int child = fork();

    if (child == -1) {
        perror("Could not fork child");
        return -1;
    } else if (child == 0) {
        if (isBackgroundJob) {
            // pid=0,gid=0
            setpgid(0, 0);
            // do not show input output error
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
        }

        if (execvp(cmd, args) < 0) {
            perror("Couldn't execute command: ");
        }

        exit(0);
    } else {
        // wait for child to complete
        if (!isBackgroundJob)
            wait(NULL);
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