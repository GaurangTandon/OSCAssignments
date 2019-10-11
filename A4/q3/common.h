#ifndef commonDone
#define commonDone

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <memory.h>
#include <pthread.h>
#include <semaphore.h>
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

#define POOL_CAB 0
#define PREMIER_CAB 1
char** CAB_STRING;

sem_t serversOpen;
int serversOpenCount;
int totalCabsOpen;

pthread_mutex_t checkCab;
pthread_mutex_t checkPayment;
short* riderWaiting;
pthread_cond_t* riderConditions;
int ridersLeftToExit;
int ridersPayingCount;

typedef struct rider {
    int cabType;
    int maxWaitTime;
    int rideTime;
    int id;
    int arrivalTime;
} rider;

typedef struct cab {
    int state, id;
} cab;

typedef struct server {
    int id;
} server;

void printTimestamp();

#endif