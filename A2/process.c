#include "process.h"
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void execProcess(char* command, char** args) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Failed fork");
    } else if (pid == 0) {
        if (execvp(command, args) < 0) {
            perror("execvp failed");
        }
        exit(0);
    } else {
        wait(NULL);
        return;
    }
}