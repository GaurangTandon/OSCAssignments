#include "rider.h"
#include "cab.h"
#include "server.h"

struct timespec* getTimeStructSinceEpoch(int extraTime) {
    struct timespec* st = (struct timespec*)malloc(sizeof(struct timespec*));

    // https://stackoverflow.com/q/46018295/
    clock_gettime(CLOCK_REALTIME, st);
    st->tv_sec += extraTime;

    return st;
}

void bookCab(rider* rider);

void riderPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(RIDER_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void* initRider(void* riderTemp) {
    rider* myrider = (rider*)riderTemp;
    myrider->maxWaitTime = rand() % MAX_WAIT_TIME + 1;
    myrider->arrivalTime = rand() % MAX_ARRIVAL_TIME + 1;
    myrider->cabType = rand() % 2;
    myrider->rideTime = rand() % MAX_RIDE_TIME + 1;
    myrider->st =
        getTimeStructSinceEpoch(myrider->arrivalTime + myrider->maxWaitTime);

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

void makePayment(rider* rider) {
    sem_post(&serversOpen);
    pthread_mutex_lock(&checkPayment);
    ridersPaying[ridersPayingCount++] = rider;
    pthread_mutex_unlock(&checkPayment);
}

cab* findCab(rider* rider) {
    cab* usedCab = NULL;

    if (rider->cabType == POOL_CAB) {
        for (int i = 0; i < cabsCount; i++) {
            if (taxis[i]->state == onRidePoolOne) {
                usedCab = taxis[i];
                usedCab->state = onRidePoolFull;
                usedCab->rider2 = rider->id;
                riderPrintMsg(rider->id,
                              "acquired cab %d of type %s, shared with %d\n",
                              usedCab->id + 1, CAB_STRING[rider->cabType],
                              usedCab->rider1 + 1);
                return usedCab;
            }
        }

        for (int i = 0; i < cabsCount; i++) {
            if (taxis[i]->state == waitState) {
                usedCab = taxis[i];
                usedCab->state = onRidePoolOne;
                usedCab->rider1 = rider->id;
                riderPrintMsg(rider->id, "acquired cab %d of type %s\n",
                              usedCab->id + 1, CAB_STRING[rider->cabType]);
                return usedCab;
            }
        }

        assert(0);
    } else {
        for (int i = 0; i < cabsCount; i++) {
            if (taxis[i]->state == waitState) {
                usedCab = taxis[i];
                usedCab->rider1 = rider->id;
                usedCab->state = onRidePremier;
                riderPrintMsg(rider->id, "acquired cab %d of type %s\n",
                              usedCab->id + 1, CAB_STRING[rider->cabType]);
                return usedCab;
            }
        }
        assert(0);
    }
}

void bookCab(rider* rider) {
    cab* usedCab = NULL;

    pthread_mutex_lock(&checkCab);

    if (totalCabsOpen > 0) {
        usedCab = findCab(rider);
    }

    if (usedCab == NULL) {
        riderPrintMsg(rider->id, "did not find any cab. Waiting.\n");
        rider->isWaiting = rider->cabType;
        int res = pthread_cond_timedwait(&rider->cond, &checkCab, rider->st);

        rider->isWaiting = -1;

        pthread_mutex_unlock(&checkCab);

        if (res == 0) {
            // riderPrintMsg(rider->id,
            //               "got signal that suitable cab is available\n");
            bookCab(rider);
            return;
        } else if (res == ETIMEDOUT) {
            riderPrintMsg(
                rider->id,
                KRED "timed out waiting for a cab (maxwaittime: %d)\n" KNRM,
                rider->maxWaitTime);
            return;
        } else {
            perror("DEBUG: ");
            return;
        }
    }

    totalCabsOpen--;
    pthread_mutex_unlock(&checkCab);

    startAndEndRide(usedCab, rider);

    riderPrintMsg(rider->id, "has left cab %d\n", usedCab->id + 1);

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