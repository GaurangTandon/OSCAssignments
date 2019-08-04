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
#define DEBUG 1
#define fileSize 1000000000  // should be INT_MAX but ok
#define PERMS S_IRUSR | S_IWUSR

// declare globally since huge size
char fileReadBuf[fileSize];
char outputBuf[fileSize];

int main() {
    char inFileName[50], outFileName[75] = FOLDER, offset = 0;

    struct stat a;
    if (stat(FOLDER, &a) == -1) {
        // TODO: ASK DOUBT IN FORUM ON 7th
        // not possible to open folder without executable permission
        mkdir(FOLDER, S_IRUSR | S_IWUSR | S_IXUSR);
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
    // remove that annoying newline :|
    inFileName[--i] = 0;
    outFileName[offset + i] = 0;

    int fdIn = open(inFileName, __O_LARGEFILE | O_RDONLY);
    if (fdIn < 0) {
        if (DEBUG) {
            printf("%s", inFileName);
            perror("Opening input file");
        }
        return 1;
    }
    int fdOut =
        open(outFileName, __O_LARGEFILE | O_CREAT | O_WRONLY | O_TRUNC, PERMS);
    if (fdOut < 0) {
        if (DEBUG) {
            printf("%s\n", outFileName);
            perror("Opening output file");
        }
        return 2;
    }
    read(fdIn, fileReadBuf, fileSize);

    int len = 0;
    while (len < fileSize && fileReadBuf[len])
        len++;
    // terminal newline :/
    len--;

    for (int i = 0, j = len - 1; i <= j; i++, j--) {
        outputBuf[i] = fileReadBuf[j];
        outputBuf[j] = fileReadBuf[i];
    }
    outputBuf[len] = 0;
    write(fdOut, outputBuf, len);

    close(fdIn);
    close(fdOut);

    /**
     * For stat, could do something like stat.st_blocks / stat.st_size
     * For continuous refresh, we use while(stat.st_blocks != stat.st_size)
     * sleep(5secs) For delete chars, use
     * https://stackoverflow.com/questions/17006262
     */

    return 0;
}

/**
 * 2147483647
 * 1000000000
 * 80000000  (for contrast, we usually have arrays of size this, 10^7 ll ints)
 */