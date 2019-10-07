#define _POSIX_C_SOURCE 199309L  // required for clock
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

int *shareMem(size_t size) {
    key_t mem_key = IPC_PRIVATE;
    // get shared memory of this much size and with this private key
    // what is 0666?
    int shm_id = shmget(mem_key, size, IPC_CREAT | 0666);

    // attach the address space of shared memory to myself (callee)
    return (int *)shmat(shm_id, NULL, 0);
}

void swap(int *arr, int i, int j) {
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

void insertionSort(int *arr, int l, int r) {
    for (int i = l; i < r; i++) {
        int j = i + 1;
        for (; j <= r; j++)
            if (arr[j] < arr[i] && j <= r) {
                swap(arr, i, j);
            }
    }
}

int partition(int *arr, int l, int r) {
    int len = r - l + 1;
    int idx = rand() % len;
    int pivot = arr[l + idx];

    int i = l - 1, j = r + 1;
    while (1) {
        do {
            i++;
        } while (i <= r && arr[i] < pivot);

        do {
            j--;
        } while (j >= l && arr[j] > pivot);

        if (i >= j) {
            return j;
        }
        swap(arr, i, j);
    }
}

void normal_quicksort(int *arr, int l, int r) {
    if (l >= r)
        return;

    // insertion sort
    if (r - l + 1 <= 5) {
        insertionSort(arr, l, r);
        return;
    }

    int pivotIdx = partition(arr, l, r);
    normal_quicksort(arr, l, pivotIdx - 1);
    normal_quicksort(arr, pivotIdx + 1, r);
}

void quicksort(int *arr, int l, int r) {
    if (l >= r)
        _exit(1);

    // insertion sort
    if (r - l + 1 <= 5) {
        insertionSort(arr, l, r);
        return;
    }

    int pivotIdx = partition(arr, l, r);

    int pid1 = fork();
    int pid2;
    if (pid1 == 0) {
        quicksort(arr, l, pivotIdx - 1);
        exit(1);
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            quicksort(arr, pivotIdx + 1, r);
            exit(1);
        } else {
            int status;
            waitpid(pid1, &status, 0);
            waitpid(pid2, &status, 0);
        }
    }
    return;
}

struct arg {
    int l;
    int r;
    int *arr;
};

void *threaded_quicksort(void *a) {
    // note that we are passing a struct to the threads for simplicity.
    struct arg *args = (struct arg *)a;

    int l = args->l;
    int r = args->r;
    int *arr = args->arr;
    if (l >= r)
        return NULL;

    // insertion sort
    if (r - l + 1 <= 5) {
        insertionSort(arr, l, r);
        return NULL;
    }

    int pivotIdx = partition(arr, l, r);

    // sort left half array
    struct arg a1;
    a1.l = l;
    a1.r = pivotIdx - 1;
    a1.arr = arr;
    pthread_t tid1;
    pthread_create(&tid1, NULL, threaded_quicksort, &a1);

    // sort right half array
    struct arg a2;
    a2.l = pivotIdx + 1;
    a2.r = r;
    a2.arr = arr;
    pthread_t tid2;
    pthread_create(&tid2, NULL, threaded_quicksort, &a2);

    // wait
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return NULL;
}

long double gettime(struct timespec *ts) {
    clock_gettime(CLOCK_MONOTONIC_RAW, ts);
    return (*ts).tv_nsec / 1e9 + (*ts).tv_sec;
}

void runSorts() {
    long long int n;
    scanf("%lld", &n);

    struct timespec ts;

    // getting shared memorya address
    int *arr = shareMem(sizeof(int) * (n + 1));
    if (*arr == -1) {
        perror("Couldn't attach memory segment");
        return;
    }
    // you are storing an already sorted array into brr. Why?
    // I changed it to store the (original) unsorted array
    int brr[n], crr[n];  // fixed dimension
    for (int i = 0; i < n; i++) {
        scanf("%d", arr + i);
        brr[i] = arr[i];
        crr[i] = arr[i];
    }

    printf("Running concurrent_quicksort for n = %lld\n", n);
    long double startTime = gettime(&ts);

    // multiprocess quicksort
    quicksort(arr, 0, n - 1);

    long double endTime = gettime(&ts);
    long double t1 = endTime - startTime;
    printf("time = %Lf\n", t1);

    pthread_t tid;
    struct arg a;
    a.l = 0;
    a.r = n - 1;
    a.arr = brr;
    printf("Running threaded_concurrent_quicksort for n = %lld\n", n);
    startTime = gettime(&ts);

    // multithreaded quicksort
    pthread_create(&tid, NULL, threaded_quicksort, &a);
    pthread_join(tid, NULL);

    endTime = gettime(&ts);
    long double t2 = endTime - startTime;
    printf("time = %Lf\n", t2);

    printf("Running normal_quicksort for n = %lld\n", n);
    startTime = gettime(&ts);

    // normal quicksort
    normal_quicksort(crr, 0, n - 1);

    endTime = gettime(&ts);
    long double t3 = endTime - startTime;
    printf("time = %Lf\n", t3);

    printf(
        "normal_quicksort ran:\n\t[ %Lf ] times faster than "
        "concurrent_quicksort\n\t[ %Lf ] times faster than "
        "threaded_concurrent_quicksort\n\n\n",
        t3 / t1, t2 / t3);
    shmdt(arr);
    return;
}

int main() {
    runSorts();
    return 0;
}