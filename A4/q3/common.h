#include <assert.h>
#include <math.h>
#include <memory.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_WAIT_TIME 100
#define MAX_RIDE_TIME 100
#define POOL_CAB 0
#define PREMIER_CAB 1
char** CAB_STRING = {"POOL", "PREMIER"};
sem_t totalCabsOpen, totalPoolCabsOpen, totalPremierCabsOpen;

int ridersInitialized = 0;