#include "rider.h"
#include "cab.h"
#include "server.h"

void bookCab(rider* rider);

struct timespec* getTimeStructSinceEpoch(int extraTime) {
    struct timespec* st = (struct timespec*)malloc(sizeof(struct timespec*));

    // https://stackoverflow.com/q/46018295/
    clock_gettime(CLOCK_REALTIME, st);
    st->tv_sec += extraTime;

    return st;
}

void riderPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(RIDER_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void* initRider(void* riderTemp) {
    rider* myrider = (rider*)riderTemp;
    myrider->maxWaitTime = rand() % MAX_WAIT_TIME;
    myrider->arrivalTime = rand() % MAX_ARRIVAL_TIME;
    myrider->cabType = rand() % 2;
    myrider->rideTime = rand() % MAX_RIDE_TIME;

    riderPrintMsg(myrider->id,
                  "initialized with rideTime %d, max wait time %d, "
                  "cabType %s and arrival time %d\n",
                  myrider->rideTime, myrider->maxWaitTime,
                  CAB_STRING[myrider->cabType], myrider->arrivalTime);
    fflush(stdout);

    sleep(myrider->arrivalTime);
    bookCab(myrider);
    return NULL;
}

void makePayment() {
    sem_post(&serversOpen);
}

void bookCab(rider* rider) {
    // do we really need this semaphore?
    // this has the issue of repeatedly acquiring and releasing the
    // semaphore which is a type of busy waiting :/ answer: without this we
    // will do busy waiting (a lot of it), so this is better

    cab* usedCab = NULL;
    struct timespec* st = getTimeStructSinceEpoch(rider->maxWaitTime);

    pthread_mutex_lock(&checkCab);
start:
    if (totalCabsOpen > 0) {
        if (rider->cabType == POOL_CAB) {
            int flag = 0;

            for (int i = 0; i < cabsCount; i++) {
                if (taxis[i]->state == onRidePoolOne) {
                    usedCab = taxis[i];
                    usedCab->state = onRidePoolFull;
                    usedCab->rider2 = rider->id;
                    riderPrintMsg(
                        rider->id,
                        "acquired cab %d of type %s, shared with %d\n",
                        usedCab->id + 1, CAB_STRING[rider->cabType],
                        usedCab->rider1 + 1);
                    flag = 1;
                    break;
                }
            }

            if (flag != 1) {
                for (int i = 0; i < cabsCount; i++) {
                    if (taxis[i]->state == waitState) {
                        usedCab = taxis[i];
                        usedCab->state = onRidePoolOne;
                        usedCab->rider1 = rider->id;
                        riderPrintMsg(rider->id, "acquired cab %d of type %s\n",
                                      usedCab->id + 1,
                                      CAB_STRING[rider->cabType]);
                        flag = 1;
                        break;
                    }
                }
            }

            assert(flag == 1);
        } else {
            int flag = 0;
            for (int i = 0; i < cabsCount; i++) {
                if (taxis[i]->state == waitState) {
                    usedCab = taxis[i];
                    usedCab->rider1 = rider->id;
                    usedCab->state = onRidePremier;
                    riderPrintMsg(rider->id, "acquired cab %d of type %s\n",
                                  usedCab->id + 1, CAB_STRING[rider->cabType]);
                    flag = 1;
                    break;
                }
            }
            assert(flag == 1);
        }
    }

    int res = !!usedCab;

    if (!res) {
        rider->isWaiting = rider->cabType;
        res = pthread_cond_timedwait(&rider->cond, &checkCab, st);
        if (res != ETIMEDOUT) {
            goto start;
        }
    }

    if (res == ETIMEDOUT) {
        rider->isWaiting = -1;
        riderPrintMsg(rider->id,
                      KRED
                      "timed out waiting for a cab (maxwaittime: %d)\n" KNRM,
                      rider->maxWaitTime);
        return;
    }

    totalCabsOpen--;
    pthread_mutex_unlock(&checkCab);

    startAndEndRide(usedCab, rider);

    riderPrintMsg(rider->id, "has left cab %d\n", usedCab->id + 1);

    pthread_mutex_lock(&checkPayment);
    ridersPaying[ridersPayingCount++] = rider;
    pthread_mutex_unlock(&checkPayment);
    makePayment(rider);
}

void madePayment(rider* rider) {
    riderPrintMsg(rider->id,
                  "has made payment, and will now exit the system\n");

    // probably there is race condition in trying to
    // modify this variable; so it does not get decremented
    // all of the time
    pthread_mutex_lock(&checkPayment);
    ridersLeftToExit--;
    pthread_mutex_unlock(&checkPayment);
}