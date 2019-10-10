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