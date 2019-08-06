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

#define fileNameSizeLimit 400
#define FOLDER "Assignment"
#define DEBUG 1
#define PERMS S_IRUSR | S_IWUSR

/**
 * TODO:
 * remove non-syscall fns
 */
long long fileSize;

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"

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

void writeProgress(int percent) {
    const int size = 102;
    char output[size];

    output[0] = output[size - 1] = '|';
    for (int i = 1; i <= percent; i++)
        output[i] = '#';
    for (int i = percent + 1; i <= 100; i++)
        output[i] = '_';

    write(1, "\r", 2);

    write(1, KGRN, 5);
    write(1, output, size);
    printnum(percent);
    write(1, KNRM, 5);

    write(1, "% of file written", 18);
}

// read file name and return its length
int readFileName(char path[fileNameSizeLimit]) {
    int len = read(0, path, fileNameSizeLimit);
    while (len >= 1 && (path[len - 1] == 3 || path[len - 1] == '\n'))
        len--;
    path[len] = 0;
    return len;
}

void reverse(char* str, int len) {
    for (int i = 0, j = len - 1; i <= j; i++, j--) {
        char t = str[i];
        str[i] = str[j];
        str[j] = t;
    }
}

int main() {
    char inFileName[fileNameSizeLimit], outFileName[fileNameSizeLimit] = FOLDER;
    long long offset = 0;

    struct stat a;
    if (stat(FOLDER, &a) == -1) {
        // TODO: ASK DOUBT IN FORUM ON 7th
        // not possible to open folder without executable permission
        mkdir(FOLDER, S_IRUSR | S_IWUSR | S_IXUSR);
    }

    readFileName(inFileName);

    // append scanned file name to get complete file name
    while (outFileName[offset])
        offset++;
    outFileName[offset++] = '/';
    int i = 0;
    for (; inFileName[i] != 0; i++) {
        outFileName[offset + i] = inFileName[i];
    }
    outFileName[offset + i] = 0;

    int fdIn = open(inFileName, __O_LARGEFILE | O_RDONLY);
    if (fdIn < 0) {
        if (DEBUG) {
            printf("%s", inFileName);
            perror("Opening input file");
        }
        return 1;
    }
    fstat(fdIn, &a);
    fileSize = a.st_size;
    int fdOut =
        open(outFileName, __O_LARGEFILE | O_CREAT | O_WRONLY | O_TRUNC, PERMS);
    if (fdOut < 0) {
        if (DEBUG) {
            printf("%s\n", outFileName);
            perror("Opening output file");
        }
        return 2;
    }

    const int multiplePrintStep = 100;
    lseek(fdIn, -multiplePrintStep, SEEK_END);
    int steps = fileSize / multiplePrintStep;
    char buf[multiplePrintStep];

    for (int i = 0; i < steps; i++) {
        read(fdIn, buf, multiplePrintStep);
        // now reverse the buf
        reverse(buf, multiplePrintStep);
        write(fdOut, buf, multiplePrintStep);
        lseek(fdIn, -2 * multiplePrintStep, SEEK_CUR);

        writeProgress((i * multiplePrintStep * 100) / fileSize);
    }
    int left = fileSize - steps * multiplePrintStep;
    // seek to beginning of file
    lseek(fdIn, -fileSize, SEEK_END);
    read(fdIn, buf, left);
    reverse(buf, left);
    write(fdOut, buf, left);
    writeProgress(100);

    close(fdIn);
    close(fdOut);

    return 0;
}
