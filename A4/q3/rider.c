#include "rider.h"
#include "cab.h"
#include "server.h"

void bookCab(rider* rider);

void* initRider(void* riderTemp) {
    rider* myrider = (rider*)riderTemp;
    myrider->id = ++ridersInitialized;
    myrider->maxWaitTime = rand() % MAX_WAIT_TIME;
    myrider->arrivalTime = rand() % MAX_ARRIVAL_TIME;
    myrider->cabType = rand() % 2;
    myrider->rideTime = rand() % MAX_RIDE_TIME;
    printf(
        "%d-th rider initialized with rideTime %d and max wait time %d and "
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
        riderWaiting[rider->id] = rider->cabType + 1;
        res =
            pthread_cond_timedwait(&riderConditions[rider->id], &checkCab, st);

        if (res != -1)
            goto start;
    }

    if (res == -1) {
        printf("Rider %d timed out waiting for a cab (maxwaittime: %d)\n",
               rider->id, rider->maxWaitTime);
        return;
    }

    // cab is booked, now start the ride
    startRide(usedCab, rider);

    // ride ended, make payment
    makePayment();
}
