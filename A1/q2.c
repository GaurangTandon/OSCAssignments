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

// read file name and return its length
int readFileName(char path[fileNameSizeLimit]) {
    int len = read(0, path, fileNameSizeLimit);
    while (len >= 1 && (path[len - 1] == 3 || path[len - 1] == '\n'))
        len--;
    path[len] = 0;
    return len;
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
            print("Directory is created: No");
            path = "Assignment";
            mkdir(path, S_IXUSR | S_IWUSR | S_IRUSR);
            ret = stat(path, &s);
        } else
            return 1;
    } else if (num == 3)
        print("Directory is created: Yes\n");

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

        if (printPerms(argv[i], i)) {
            return i;
        }
    }

    return 0;
}