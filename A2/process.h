#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void execProcess(char* cmd, char** args, int isBackgroundJob);