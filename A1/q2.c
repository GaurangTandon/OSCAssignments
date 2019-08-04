#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define fileNameSizeLimit 100
char newFilePath[fileNameSizeLimit], oldFilePath[fileNameSizeLimit],
    directoryPath[fileNameSizeLimit];

int main() {
    read(0, newFilePath, fileNameSizeLimit);
    // read(0, oldFilePath, fileNameSizeLimit);
    // read(0, directoryPath, fileNameSizeLimit);

    struct stat s;
    write(1, "Directory is created: ", 23);
    if (stat(directoryPath, &s) == -1) {
        write(1, "No\n", 4);
    } else {
        write(1, "Yes\n", 5);
    }

    return 0;
}