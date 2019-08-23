#include "sysData.h"
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

char* getUser() {
    struct passwd* p = getpwuid(getuid());
    return p->pw_name;
}

void printMachine() {
    int fd = open("/proc/sys/kernel/hostname", O_RDONLY);
    if (fd < 0)
        return;
    char name[100];
    int len = read(fd, name, 100);
    name[len - 1] = 0;
    printf("%s", name);
    close(fd);
}
