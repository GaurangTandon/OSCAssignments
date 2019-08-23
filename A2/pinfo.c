#include "pinfo.h"
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "directory.h"

void printPinfo(int argCount, char** args) {
    int pid = getpid();
    int len = log10(pid) + 1;
    char* str = (char*)malloc(len * sizeof(char));
    sprintf(str, "%d", pid);

    int isSelf = 1;

    if (argCount == 2) {
        str = args[1];
        isSelf = 0;
    }
    printf("PID:\t\t\t%s\n", str);

    char path[100] = "/proc/";
    strcat(path, str);
    strcat(path, "/stat");

    int statfile = open(path, O_RDONLY);
    char* buf = (char*)malloc(1000);
    read(statfile, buf, 10000);
    char* ptr = strtok(buf, " ");
    ptr = strtok(NULL, " ");
    ptr = strtok(NULL, " ");

    printf("Process status:\t\t%s\n", ptr);
    close(statfile);

    strcat(path, "m");
    statfile = open(path, O_RDONLY);
    buf = (char*)malloc(1000);
    read(statfile, buf, 10000);
    ptr = strtok(buf, " ");
    printf("Memory:\t\t\t%s\n", ptr);
    close(statfile);

    char path2[100] = "/proc/";
    strcat(path2, str);
    strcat(path2, "/exe");
    buf = (char*)malloc(1000);
    readlink(path2, buf, 1000);

    if (isSelf) {
        printf("Executable path: \t");
        printPWD(0);
        printf("/a.out");
    } else
        printf("Executable path: \t%s", buf);
    printf("\n");
    fflush(stdout);
}