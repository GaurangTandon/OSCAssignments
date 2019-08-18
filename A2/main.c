// this file is the core for interacting with the user

#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

void printUser() {
    struct passwd* p = getpwuid(getuid());
    char* usrName = p->pw_name;
    printf("%s", usrName);
}

void printMachine() {
    struct utsname buf;
    uname(&buf);
    char* osName = buf.sysname;
    printf("%s", osName);
}

void printPrompt() {
    printUser();
    printf("@");
    printMachine();
}

int main() {
    printPrompt();
}