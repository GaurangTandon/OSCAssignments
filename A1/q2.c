#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

int isReved(char a[], char b[], int len) {
    for (int i = 0, j = len - 1; i < len; i++, j--) {
        if (a[i] != b[j])
            return 0;
    }
    return 1;
}

int reversed(char* newFilePath, char* oldFilePath) {
    int fd1 = open(newFilePath, O_RDONLY);
    if (fd1 < 0) {
        perror("New file path issue");
        return -1;
    }
    int fd2 = open(oldFilePath, O_RDONLY);
    if (fd2 < 0) {
        perror("Old file path issue");
        return -1;
    }
    struct stat s1, s2;
    stat(newFilePath, &s1);
    stat(oldFilePath, &s2);
    if (s1.st_size != s2.st_size) {
        return 0;
    }

    int fileSize = s1.st_size;

    // reed 1e6 characters at once, and compare
    const long long int multiplePrintStep =
                            max(min(fileSize / 1000, (long long int)1e6), 1),
                        steps = fileSize / multiplePrintStep;

    char buf[multiplePrintStep], buf2[multiplePrintStep];

    lseek(fd1, -multiplePrintStep, SEEK_END);
    for (int i = 0; i < steps; i++) {
        read(fd1, buf, multiplePrintStep);
        read(fd2, buf2, multiplePrintStep);
        if (!isReved(buf, buf2, multiplePrintStep)) {
            return 0;
        }

        lseek(fd1, -2 * multiplePrintStep, SEEK_CUR);
    }

    int left = fileSize - steps * multiplePrintStep;
    // seek to beginning of file
    lseek(fd1, 0, SEEK_SET);
    read(fd1, buf, left);
    read(fd2, buf2, left);

    if (!isReved(buf, buf2, left)) {
        return 0;
    }

    return 1;
}

void print(char str[]) {
    int len = 0;
    while (str[len])
        len++;
    write(1, str, len);
}

char* proc;

int printPerms(char path[], int num) {
    struct stat s;
    int ret = stat(path, &s);
    if (ret < 0) {
        if (num == 3) {
            print("Directory is created: No\n");
            return 1;
        } else {
            perror(path);
            return 1;
        }
    } else if (num == 3) {
        if (S_ISDIR(s.st_mode))
            print("Directory is created: Yes\n");
        else
            print("Directory is created: It is not a directory :|\n");
    }

    int perms[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                    S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

    for (int i = 0; i < 9; i++) {
        if (i < 3)
            print("User");
        else if (i < 6)
            print("Group");
        else
            print("Others");
        print(" has ");
        if (i % 3 == 0)
            print("read");
        else if (i % 3 == 1)
            print("write");
        else
            print("execute");
        print(" permission on ");
        print(proc);
        print(": ");

        if (s.st_mode & perms[i])
            print("Yes");
        else
            print("No");
        print("\n");
    }
    print("--\n");

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        write(1,
              "Please pass three path string arguments in this order: newfile, "
              "oldfile and the directory",
              90);
        return 1;
    }
    for (int i = 1; i <= 3; i++) {
        if (i == 1) {
            proc = "newfile";
        } else if (i == 2)
            proc = "oldfile";
        else
            proc = "directory";

        printPerms(argv[i], i);
    }

    print("Whether file contents are reversed in newfile: ");
    int res = reversed(argv[1], argv[2]);
    if (res == -1) {
    } else if (res == 1) {
        print("Yes");
    } else
        print("No");

    return 0;
}