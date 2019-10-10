#ifndef commonDone
#define commonDone

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
char** CAB_STRING;

sem_t serversOpen;
int serversOpenCount;
int totalCabsOpen, totalPoolCabsOpen, totalPremierCabsOpen;

pthread_mutex_t checkCab;
pthread_mutex_t riderMutexes[MAX_RIDERS];
short riderWaiting[MAX_RIDERS];
pthread_cond_t riderConditions[MAX_RIDERS];  // initialized in main.c

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

#endif