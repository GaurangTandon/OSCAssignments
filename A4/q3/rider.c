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

void printRiderHead(int id) {
    printTimestamp();
    printf(KMAGENTA "Rider %d\t\t" KNRM, id);
}

void* initRider(void* riderTemp) {
    rider* myrider = (rider*)riderTemp;
    myrider->maxWaitTime = rand() % MAX_WAIT_TIME;
    myrider->arrivalTime = rand() % MAX_ARRIVAL_TIME;
    myrider->cabType = rand() % 2;
    myrider->rideTime = rand() % MAX_RIDE_TIME;

    printRiderHead(myrider->id);
    printf(
        "initialized with rideTime %d, max wait time %d, "
        "cabType %s and arrival time %d\n",
        myrider->rideTime, myrider->maxWaitTime, CAB_STRING[myrider->cabType],
        myrider->arrivalTime);
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
            if (poolOneCabs[0]) {
                usedCab = poolOneCabs[0];
                usedCab->state = onRidePoolOne;
            } else {
                usedCab = waitingCabs[0];
                usedCab->state = onRidePremier;
            }
        } else if (waitingCabs[0]) {
            usedCab = waitingCabs[0];
            usedCab->state = onRidePremier;
        }

        printRiderHead(rider->id);
        printf("acquired cab %d of type %s\n", usedCab->id,
               CAB_STRING[rider->cabType]);
    }

    int res = !!usedCab;

    if (!res) {
        riderWaiting[rider->id] = rider->cabType;
        res =
            pthread_cond_timedwait(&riderConditions[rider->id], &checkCab, st);
        if (res != ETIMEDOUT) {
            goto start;
        }
    }

    if (res == ETIMEDOUT) {
        printRiderHead(rider->id);
        printf("timed out waiting for a cab (maxwaittime: %d)\n",
               rider->maxWaitTime);
        pthread_mutex_unlock(&checkCab);
        return;
    }

    startAndEndRide(usedCab, rider);

    printRiderHead(rider->id);
    printf("has left the cab\n");

    pthread_mutex_lock(&checkPayment);
    ridersPaying[ridersPayingCount++] = rider;
    pthread_mutex_unlock(&checkPayment);
    makePayment(rider);
}

void madePayment(rider* rider) {
    printRiderHead(rider->id);
    printf("has made payment, and will now exit the system\n");

    // probably there is race condition in trying to
    // modify this variable; so it does not get decremented
    // all of the time
    pthread_mutex_lock(&checkPayment);
    ridersLeftToExit--;
    pthread_mutex_unlock(&checkPayment);
}