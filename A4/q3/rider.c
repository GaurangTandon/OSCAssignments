#include "rider.h"
#include "cab.h"
#include "server.h"

void bookCab(rider* rider);

struct timespec* getTimeStructSinceEpoch(int extraTime) {
    time_t passed;
    time(&passed);

    struct timespec* st = (struct timespec*)malloc(sizeof(struct timespec*));
    st->tv_sec = passed + extraTime;
    st->tv_nsec = 0;
    return st;
}

void* initRider(void* riderTemp) {
    rider* myrider = (rider*)riderTemp;
    myrider->maxWaitTime = rand() % MAX_WAIT_TIME;
    myrider->arrivalTime = rand() % MAX_ARRIVAL_TIME;
    myrider->cabType = rand() % 2;
    myrider->rideTime = rand() % MAX_RIDE_TIME;
    printf(
        "Rider id %d initialized with rideTime %d, max wait time %d, "
        "cabType %s and arrival time %d\n",
        myrider->id, myrider->rideTime, myrider->maxWaitTime,
        CAB_STRING[myrider->cabType], myrider->arrivalTime);
    sleep(myrider->arrivalTime);
    bookCab(myrider);
    return NULL;
}

void bookCab(rider* rider) {
    // do we really need this semaphore?
    // this has the issue of repeatedly acquiring and releasing the
    // semaphore which is a type of busy waiting :/ answer: without this we
    // will do busy waiting (a lot of it), so this is better

    cab* usedCab = NULL;
    struct timespec* st = getTimeStructSinceEpoch(rider->maxWaitTime);

    // entering CS
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

        printf("Rider %d has acquired cab %d of type %s\n", rider->id, -999,
               CAB_STRING[0]);
    }

    int res = !!usedCab;

    if (!usedCab) {
        pthread_mutex_unlock(&checkCab);

        pthread_mutex_lock(&riderMutexes[rider->id]);
        riderWaiting[rider->id] = rider->cabType + 1;
        res = pthread_cond_timedwait(&riderConditions[rider->id],
                                     &riderMutexes[rider->id], st);

        if (res != -1)
            goto start;
    }

    if (res == -1) {
        printf("Rider %d timed out waiting for a cab (maxwaittime: %d)\n",
               rider->id, rider->maxWaitTime);
        return;
    }

    // cab is booked, now start the ride
    startAndEndRide(usedCab, rider);

    printf("Rider %d has got down to make payment\n", rider->id);

    makePayment();

    printf("Rider %d has made payment. He will now exit the system\n",
           rider->id);
}
