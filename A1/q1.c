// syscall functions are found in unistd.h
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FOLDER "Assignment"
#define DEBUG 1
#define PERMS S_IRUSR | S_IWUSR

/**
 * TODO: implement progress bar
 * remove non-syscall fns
 * use stat syscall to find out file size, and then malloc those many bytes
 * write byte by byte (by seeking) to give a progress indicator kind of thing
 */
// declare globally since huge size
char *fileReadBuf, *outputBuf;
long long fileSize;

void printnum(int num) {
    char arr[100];
    int digs = 0;
    while (num > 0) {
        arr[digs++] = (num % 10) + '0';
        num /= 10;
    }
    for (int i = 0, j = digs - 1; i <= j; i++, j--) {
        char x = arr[i];
        arr[i] = arr[j];
        arr[j] = x;
    }
    write(1, arr, digs);
}

int main() {
    char inFileName[100], outFileName[100] = FOLDER;
    long long offset = 0;

    struct stat a;
    if (stat(FOLDER, &a) == -1) {
        // TODO: ASK DOUBT IN FORUM ON 7th
        // not possible to open folder without executable permission
        mkdir(FOLDER, S_IRUSR | S_IWUSR | S_IXUSR);
    }

    // pattern: file descriptor fd, buffer name, upto count bytes
    read(0, inFileName, 100);

    // append scanned file name to get complete file name
    while (outFileName[offset])
        offset++;
    outFileName[offset++] = '/';
    int i = 0;
    for (; inFileName[i] != 0; i++) {
        outFileName[offset + i] = inFileName[i];
    }
    // remove that annoying newline :|
    // ascii 3 is eot
    while (i >= 1 && (inFileName[i - 1] == 3 || inFileName[i - 1] == '\n'))
        i--;
    inFileName[i] = 0;
    outFileName[offset + i] = 0;

    // this printf shouldn't have a newline
    int fdIn = open(inFileName, __O_LARGEFILE | O_RDONLY);
    if (fdIn < 0) {
        if (DEBUG) {
            printf("%sabs", inFileName);
            fflush(stdout);
            perror("Opening input file");
        }
        return 1;
    }
    fstat(fdIn, &a);
    fileSize = a.st_size;
    fileReadBuf = (char *)malloc(fileSize * 1);
    outputBuf = (char *)malloc(fileSize * 1);
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
    while (len >= 1 &&
           (fileReadBuf[len - 1] == 3 || fileReadBuf[len - 1] == '\n'))
        len--;
    fileReadBuf[len] = 0;
    for (int i = 0, j = len - 1; i <= j; i++, j--) {
        outputBuf[i] = fileReadBuf[j];
        outputBuf[j] = fileReadBuf[i];
    }
    outputBuf[len] = 0;

    int multiplePrintStep = 100;
    for (int i = 0; i <= len; i++) {
        write(fdOut, outputBuf + i, 1);
        lseek(fdOut, 1, SEEK_CUR);
        write(1, "\r", 2);

        if (i % multiplePrintStep == 0) {
            printnum((i * 100) / len);
            write(1, "% of file written", 18);
        }
        fflush(stdout);
    }

    write(1, "\r100% of file written", 22);
    fflush(stdout);

    close(fdIn);
    close(fdOut);

    return 0;
}

/**
 * 2147483647
 * 1000000000
 * 80000000  (for contrast, we usually have arrays of size this, 10^7 ll ints)
 */