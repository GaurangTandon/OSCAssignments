#include <assert.h>
#include <math.h>
#include <memory.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define MAX_WAIT_TIME 100
#define MAX_RIDE_TIME 100
#define MAX_ARRIVAL_TIME 100
#define MAX_CABS 100
#define MAX_RIDERS 100
#define MAX_SERVERS 100

#define POOL_CAB 0
#define PREMIER_CAB 1
char** CAB_STRING = {"POOL", "PREMIER"};

sem_t totalCabsOpen, totalPoolCabsOpen, totalPremierCabsOpen, serversOpen;
int serversOpenCount;

int ridersInitialized = 0;

struct timespec* getTimeStructSinceEpoch(int extraTime) {
    time_t passed;
    time(&passed);

    struct timespec* st = (struct timespec*)malloc(sizeof(struct timespec*));
    st->tv_sec = passed + extraTime;
    st->tv_nsec = 0;
    return st;
}