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
#define MAX_ROBOTS 100
#define MAX_STUDENTS 100
#define MAX_TABLES 100

#define ROBOT_TYPE 1
#define TABLE_TYPE 2
#define STUDENT_TYPE 3

typedef struct table {
    int id;
    int biryaniAmountRemaining;
} table;

typedef struct robot {
    int id;
    int biryaniVesselsRemaining;
    int isReady;
} robot;

typedef struct student {
    int id;
} student;

char* getTimestamp();
char* getHeader(int type, int id);
void printMsg(int type, int id, char* fmt, va_list arg);
int genRandomInRange(int l, int r);

#endif