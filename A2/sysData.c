#include "sysData.h"
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
    FILE* fp;
    char buffer[50] = " ";
    fp = popen("lsb_release -ds", "r");
    fgets(buffer, 50, fp);
    char* osName = strtok(buffer, " ");
    pclose(fp);
    printf("%s", osName);
}
