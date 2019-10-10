#include "rider.h"

void* initRider(rider* rider) {
    rider->id = ++ridersInitialized;
    rider->maxWaitTime = rand() % MAX_WAIT_TIME;
    rider->cabType = rand() % 2;
    rider->rideTime = rand() % MAX_RIDE_TIME;
    printf(
        "%d-th rider initialized with rideTime %d and max wait time %d and "
        "cabType %s\n",
        rider->id, rider->rideTime, rider->maxWaitTime,
        CAB_STRING[rider->cabType]);
    bookCab(rider);
}

void bookCab(rider* rider) {
    // do we really need this semaphore?
    // this has the issue of repeatedly acquiring and releasing the
    // semaphore which is a type of busy waiting :/ answer: without this we
    // will do busy waiting (a lot of it), so this is better

    sem_t* used;
    int res;
    struct timespec st;
    // set to rider->maxWaitTime

    if (rider->cabType == POOL_CAB) {
        res = sem_timedwait(&totalCabsOpen, st);
        if (res == -1)
            goto out;

        // for pool cab both options are open
        used = &totalPoolCabsOpen;
        int x = sem_trywait(used);

        if (x == -1) {
            used = &totalPremierCabsOpen;
            x = sem_trywait(used);
        }

        // x cannot be negative, since we waited on total cabs open
        assert(x != -1);

        printf("Pool rider %d has acquired cab %d\n", rider->id, -999);
    } else {
        used = &totalPremierCabsOpen;
        res = sem_timedwait(used, st);
        if (res == -1)
            goto out;
        // TODO: at this point I need to be forcibly decrement the totalcabsopen
        // semaphore too
        // waiting on totalCabsOpen and then on totalPremierCabsOpen is useless
        // since that results in busy waiting since multiple riders might enter
        // race condition with it
    }
out:;
    if (res == -1) {
        printf("Rider %d timed out waiting for a cab (maxwaittime: %d)\n",
               rider->id, rider->maxWaitTime);
        return;
    }
    // used contains the cab
}

void makePayment() {
}