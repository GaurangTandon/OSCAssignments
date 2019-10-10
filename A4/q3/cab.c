#include "cab.h"

void* initCab(void* cabTemp) {
    cab* mycab = (cab*)cabTemp;
    mycab->state = waitState;
    return NULL;
}

// TODO: move around these cabs in the respective arrays
// if required
// what should be the ride time if pooled one after the other?

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

void endRide(cab* cab) {
    assert(cab->state != waitState);

    if (cab->state == onRidePremier)
        cab->state = waitState;
    else
        cab->state--;
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

void startRide(cab* cab, rider* rider) {
    shiftCabsAround(cab);

}

cab* getFreeCab(int cabType) {
    if (cabType == POOL_CAB) {
        return poolOneCabs[0];
    } else {
        return waitingCabs[0];
    }
}