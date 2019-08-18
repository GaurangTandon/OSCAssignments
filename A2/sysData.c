#include "sysData.h"
#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

char* getUser() {
    struct passwd* p = getpwuid(getuid());
    return p->pw_name;
}

void printMachine() {
    struct utsname buf;
    uname(&buf);
    char* osName = buf.sysname;
    printf("%s", osName);
}
