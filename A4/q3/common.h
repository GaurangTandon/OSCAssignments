#ifndef commonDone
#define commonDone

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <memory.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "colors.h"

#define MAX_WAIT_TIME 10
#define MAX_RIDE_TIME 5
#define MAX_ARRIVAL_TIME 5
#define MAX_CABS 100
#define MAX_RIDERS 100
#define MAX_SERVERS 100

#define CAB_TYPE 0
#define RIDER_TYPE 1
#define SERVER_TYPE 2

#define POOL_CAB 0
#define PREMIER_CAB 1
char** CAB_STRING;

int cabsCount, ridersCount, serversCount;

sem_t serversOpen;
int serversOpenCount;
int totalCabsOpen;

pthread_mutex_t checkCab;
pthread_mutex_t checkPayment;
int ridersLeftToExit;
int ridersPayingCount;

typedef struct rider {
    int cabType;
    int maxWaitTime;
    int rideTime;
    int id;
    struct timespec* st;
    int arrivalTime;
    pthread_cond_t cond;
    short isWaiting;
} rider;

typedef struct cab {
    int state, id;
    int rider1, rider2;
} cab;

typedef struct server {
    int id;
} server;

void printTimestamp();
void printMsg(int type, int id, char* fmt, va_list arg);
#endif