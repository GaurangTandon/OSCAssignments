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
#define max(x, y) ((x) > (y) ? (x) : (y))

/**
 * TODO:
 * remove non-syscall fns
 */
long long fileSize;

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"

void printnum(int num) {
    char arr[100] = {0};
    int digs = 0;
    if (num == 0)
        digs = 1;
    while (num > 0) {
        arr[digs++] = (num % 10) + '0';
        num /= 10;
    }
    for (int i = 0, j = digs - 1; i <= j; i++, j--) {
        char x = arr[i];
        arr[i] = arr[j];
        arr[j] = x;
    }
    arr[digs] = 0;
    write(1, arr, digs + 1);
}

int min(int a, int b) {
    return a < b ? a : b;
}

void writeProgress(int percent) {
    write(1, "\r", 2);

    write(1, KGRN, 5);
    printnum(percent);
    write(1, KNRM, 5);

    write(1, "% of file written", 18);
    fflush(stdout);
}

void reverse(char* str, int len) {
    for (int i = 0, j = len - 1; i <= j; i++, j--) {
        char t = str[i];
        str[i] = str[j];
        str[j] = t;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        write(1,
              "Unexpected number of arguments. Put exactly one string argument "
              "indicating file name of file to reverse.",
              105);
        return 1;
    }

    char *inFileName = argv[1], outFileName[fileNameSizeLimit] = FOLDER;
    long long offset = 0;

    struct stat a;
    if (stat(FOLDER, &a) == -1) {
        mkdir(FOLDER, S_IRUSR | S_IWUSR | S_IXUSR);
    }

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
        perror("Error input file");
        return 1;
    }
    fstat(fdIn, &a);
    fileSize = a.st_size;
    int fdOut =
        open(outFileName, __O_LARGEFILE | O_CREAT | O_WRONLY | O_TRUNC, PERMS);

    if (fdOut < 0) {
        perror("Error output file");
        return 1;
    }

    const int multiplePrintStep = max(min(fileSize / 1000, (int)1e5), 1);
    int steps = fileSize / multiplePrintStep, updateStep = max(1, steps / 10);

    char buf[multiplePrintStep];

    lseek(fdIn, -multiplePrintStep, SEEK_END);
    for (int i = 0; i < steps; i++) {
        read(fdIn, buf, multiplePrintStep);
        reverse(buf, multiplePrintStep);
        write(fdOut, buf, multiplePrintStep);
        lseek(fdIn, -2 * multiplePrintStep, SEEK_CUR);

        if (i % updateStep == 0) {
            writeProgress((i * multiplePrintStep * 100) / fileSize);
        }
    }

    int left = fileSize - steps * multiplePrintStep;
    // seek to beginning of file
    lseek(fdIn, 0, SEEK_SET);
    read(fdIn, buf, left);
    reverse(buf, left);
    write(fdOut, buf, left);
    writeProgress(100);

    close(fdIn);
    close(fdOut);

    return 0;
}
