#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define fileNameSizeLimit 400
char newFilePath[fileNameSizeLimit], oldFilePath[fileNameSizeLimit],
    directoryPath[fileNameSizeLimit];

// read file name and return its length
int readFileName(char path[]) {
    read(0, path, fileNameSizeLimit);
    int len = 0;
    while (path[len])
        len++;
    while (len >= 1 && (path[len - 1] == 3 || path[len - 1] == '\n'))
        len--;
    path[len] = 0;
    return len;
}

int main() {
    int f1 = readFileName(newFilePath), f2 = readFileName(oldFilePath),
        f3 = readFileName(directoryPath);

    struct stat s;
    write(1, "Directory is created: ", 23);
    if (stat(directoryPath, &s) == -1) {
        write(1, "No\n", 4);
        perror("Error");
    } else {
        write(1, "Yes\n", 5);
    }

    return 0;
}