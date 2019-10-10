#include "cab.h"

void* initCab(void* cabTemp) {
    cab* mycab = (cab*)cabTemp;
    mycab->state = waitState;
    printf("Cab id %d initialized in wait state\n", mycab->id);
    return NULL;
}

// TODO: what should be the ride time if pooled one after the other?

void acceptRide(cab* cab, int rideType, int rideTime) {
    if (rideType == POOL_CAB) {
        assert(cab->state != oneRidePoolFull);
        cab->state++;
    } else {
        assert(cab->state == waitState);
        cab->state = onRidePremier;
    }
    sleep(rideTime);
}

void endRide(cab* cab, rider* rider) {
    assert(cab->state != waitState);

    if (cab->state == onRidePremier) {
        cab->state = waitState;
    } else {
        cab->state--;
    }

    pthread_mutex_lock(&checkCab);

    if (cab->state == onRidePoolOne) {
        int i = 0;
        while (poolOneCabs[i])
            i++;
        poolOneCabs[i] = cab;
    } else {
        int i = 0;
        while (waitingCabs[i])
            i++;
        waitingCabs[i] = cab;
    }

    int j = rider->id + 1;
    while (j != rider->id && !riderWaiting[j]) {
        j = (j + 1) % MAX_RIDERS;
    }

    if (j == rider->id) {
        pthread_mutex_unlock(&checkCab);
    } else {
        pthread_cond_signal(&riderConditions[j]);
    }
}

void shiftCabsAround(cab* cab) {
    // delete this cab from the array
    // it will always be at the 0th index
    for (int i = 0; i < MAX_CABS - 1; i++) {
        if (cab->state == onRidePremier) {
            waitingCabs[i] = waitingCabs[i + 1];
        } else {
            poolOneCabs[i] = poolOneCabs[i + 1];
        }
    }

    if (cab->state == onRidePoolOne) {
        int i = 0;
        while (poolOneCabs[i])
            i++;
        poolOneCabs[i] = cab;
    }
}

void startAndEndRide(cab* cab, rider* rider) {
    shiftCabsAround(cab);

    printf("Rider %d has started riding in cab %d\n", rider->id, cab->id);

    sleep(rider->rideTime);

    endRide(cab, rider);
}

cab* getFreeCab(int cabType) {
    if (cabType == POOL_CAB) {
        return poolOneCabs[0];
    } else {
        return waitingCabs[0];
    }
}