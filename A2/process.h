#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int execProcess(char* cmd, char** args, int isBackgroundJob);
void interruptPrint();
void dirtyMemPrint();