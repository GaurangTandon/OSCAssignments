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

char* getProcName(int pid) {
    char path[100] = "/proc/";
    char p2[100] = {0};
    snprintf(p2, 10, "%d", pid);
    strcat(path, p2);
    strcat(path, "/stat");

    int fd = open(path, O_RDONLY, 0644);
    if (fd < 0) {
        perror("Couldn't open proc file");
        return NULL;
    }

    char buf[1000], *name = (char*)calloc(1000, 1);
    read(fd, buf, 1000);

    int x = strlen(buf + 1);
    // printf("%s\n", buf);
    char* ptr = strtok(buf, " ");
    ptr = strtok(NULL, " ");
    memcpy(name, ptr + 1, x);
    name[x - 2] = 0;
    return name;
}

char* getProcStatus(char* pid) {
    char path[100] = "/proc/";
    strcat(path, pid);
    strcat(path, "/stat");

    int statfile = open(path, O_RDONLY);
    char* buf = (char*)malloc(1000);
    read(statfile, buf, 10000);
    char* ptr = strtok(buf, " ");
    ptr = strtok(NULL, " ");
    ptr = strtok(NULL, " ");
    close(statfile);
    return ptr;
}

void printPinfo(int argCount, char** args) {
    int pid = getpid();
    char* pid_str = (char*)malloc(100);
    snprintf(pid_str, 100, "%d", pid);
    int len = log10(pid) + 1;
    char* str = (char*)malloc(len * sizeof(char));
    sprintf(str, "%d", pid);

    int isSelf = 1;

    if (argCount == 2) {
        str = args[1];
        isSelf = 0;
    }
    printf("PID:\t\t\t%s\n", str);

    printf("Process status:\t\t%s\n", getProcStatus(pid_str));
    char* buf;

    char path[100] = "/proc/";
    strcat(path, pid_str);
    strcat(path, "/stat");
    strcat(path, "m");
    int statfile = open(path, O_RDONLY);
    buf = (char*)malloc(1000);
    read(statfile, buf, 10000);
    char* ptr = strtok(buf, " ");
    printf("Memory:\t\t\t%s\n", ptr);
    close(statfile);

    char path2[100] = "/proc/";
    strcat(path2, str);
    strcat(path2, "/exe");
    buf = (char*)malloc(1000);
    readlink(path2, buf, 1000);

    if (isSelf) {
        printf("Executable path: \t");
        printPWD(0, 0);
        printf("/a.out");
    } else
        printf("Executable path: \t%s", buf);
    printf("\n");
    fflush(stdout);
}

char* getProcPath(char* pid) {
    char path2[100] = "/proc/";
    strcat(path2, pid);
    strcat(path2, "/exe");
    char* buf = (char*)malloc(1000);
    readlink(path2, buf, 1000);

    return buf;
}

char* getProcStatusString(char* pid) {
    char* stat = getProcStatus(pid);

    switch (stat[0]) {
        case 'R':
            return "Running";
            break;
        case 'S':
        case 'D':
            return "Sleeping";
        case 'T':
        case 't':
            return "Stopped";
        case 'Z':
            return "Defunct/zombie";
        default:
            return "Unknown process status";
            break;
    }
}