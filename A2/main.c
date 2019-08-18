// this file is the core for interacting with the user

#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"

void printWelcomeScreen() {
    printf("Welcome to " KGRN "GOSH!" KNRM " aka GOrang's SHell :)\n");
}

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
    printWelcomeScreen();
    printPrompt();
}