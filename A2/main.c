// this file is the core for interacting with the user

#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"

char* getUser() {
    struct passwd* p = getpwuid(getuid());
    return p->pw_name;
}

char* USER;

char* currDirectories[100] = {
    "home",
};
// -1 indicates root
int currDirectory = 0;

void initSetup() {
    USER = getUser();
    currDirectories[1] = USER;
    currDirectory++;
}

void printWelcomeScreen() {
    printf("Welcome to " KGRN "GOSH!" KNRM " aka GOrang's SHell :)\n");
}

void printUser() {
    printf("%s", USER);
}

void printMachine() {
    struct utsname buf;
    uname(&buf);
    char* osName = buf.sysname;
    printf("%s", osName);
}

// we cannot use getcwd since the directory from which
// our program is executed is supposed to be the home directory
void printPWD() {
    if (currDirectory >= 1) {
        printf("~");
        for (int i = 2; i <= currDirectory; i++) {
            printf("/%s", currDirectories[i]);
        }
    } else {
        printf("/");
        if (currDirectory == 0) {
            printf("home");
        }
    }
}

void printPrompt() {
    initSetup();
    printUser();
    printf("@");
    printMachine();
    printf(":");
    printPWD();
    printf("$ ");
}

int main() {
    printWelcomeScreen();
    printPrompt();
}