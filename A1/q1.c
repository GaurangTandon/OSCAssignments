// syscall functions are found in unistd.h
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FOLDER "Assignment"
#define DEBUG true
#define fileSize 1000000000  // should be INT_MAX but ok
#define PERMS S_IRUSR | S_IWUSR

// declare globally since huge size
char fileReadBuf[fileSize];
char outputBuf[fileSize];

int main() {
    char inFileName[50], outFileName[75] = FOLDER, offset = 0;

    struct stat a;
    if (stat(FOLDER, &a) == -1) {
        mkdir(FOLDER, PERMS);
    }

    // pattern: file descriptor fd, buffer name, upto count bytes
    read(0, inFileName, 50);

    // append scanned file name to get complete file name
    while (outFileName[offset])
        offset++;
    outFileName[offset++] = '/';
    int i = 0;
    for (; inFileName[i] != 0; i++) {
        outFileName[offset + i] = inFileName[i];
    }
    outFileName[offset + i] = 0;

    if (DEBUG)
        write(1, outFileName, 75);  // for debug

    // now to open a large file, create if not exists, and with necessary
    // permissions
    int fdIn = open(inFileName, O_LARGEFILE | O_RDONLY),
        fdOut =
            open(outFileName, O_LARGEFILE | O_CREAT | O_RDWR | O_TRUNC, PERMS);
    if (fdIn < 0) {
        if (DEBUG)
            perror("Opening input file");
        return 1;
    }
    if (fdOut < 0) {
        if (DEBUG)
            perror("Opening output file");
        return 2;
    }
    read(fdIn, fileReadBuf, fileSize);

    int len = 0;
    while (len < fileSize && fileReadBuf[len])
        len++;

    for (int i = 0, j = len - 1; i < j; i++, j--) {
        outputBuf[i] = fileReadBuf[j];
        outputBuf[j] = fileReadBuf[i];
    }

    write(fdOut, outputBuf, len);

    close(fdIn);
    close(fdOut);

    return 0;
}

/**
 * 2147483647
 * 1000000000
 * 80000000  (for contrast, we usually have arrays of size this, 10^7 ll ints)
 */