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

#define MAX_ARRIVAL_TIME 5
#define MAX_ROBOTS 100
#define MAX_STUDENTS 100
#define MAX_TABLES 100

#define ROBOT_TYPE 1
#define TABLE_TYPE 2
#define STUDENT_TYPE 3

typedef struct table {
    int id;
    int biryaniAmountRemaining;
    int needVessel;
    int readyToServe;
    int slotsLeft;
    int studentsEatingHere[11];
} table;

typedef struct robot {
    int id;
    int biryaniVesselsRemaining;
    int isReady;
    int vesselSize;
} robot;

typedef struct student {
    int id;
    int arrivalTime;
} student;

pthread_mutex_t* checkTable;
pthread_mutex_t* checkRobot;
pthread_cond_t* tableConditions;
pthread_mutex_t* tableMutexes;
pthread_mutex_t updateMutex;
pthread_cond_t* robotConditions;
pthread_mutex_t* robotMutexes;

char* getTimestamp();
char* getHeader(int type, int id);
void printMsg(int type, int id, char* fmt, va_list arg);
int genRandomInRange(int l, int r);
int robotCount, studentCount, tableCount;

// cannot use such global variables because
// we are not allowed to use a global mutex
// int studentsLeftCount;

#endif