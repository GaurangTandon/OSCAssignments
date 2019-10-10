#include "rider.h"
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

    sem_t* used;
    int res;
    struct timespec* st = getTimeStructSinceEpoch(rider->maxWaitTime);

    int bookedCab;
    if (rider->cabType == POOL_CAB) {
        res = sem_timedwait(&totalCabsOpen, st);
        if (res == -1)
            goto out;

        // for pool cab both options are open
        used = &totalPoolCabsOpen;
        int x = sem_trywait(used);
        bookedCab = POOL_CAB;

        if (x == -1) {
            used = &totalPremierCabsOpen;
            x = sem_trywait(used);
            bookedCab = PREMIER_CAB;
        }

        // x cannot be negative, since we waited on total cabs open
        assert(x != -1);

        printf("Pool rider %d has acquired cab %d of type %s\n", rider->id,
               -999, CAB_STRING[bookedCab]);
    } else {
        used = &totalPremierCabsOpen;
        res = sem_timedwait(used, st);
        bookedCab = PREMIER_CAB;
        if (res == -1)
            goto out;
        printf("Premier rider %d has acquired cab %d\n", rider->id, -999);
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

    // cab is booked, now start the ride

    sem_post(used);
    sem_post(&totalCabsOpen);

    makePayment();
}
