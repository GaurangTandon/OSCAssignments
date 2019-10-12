#include "cab.h"
#include "rider.h"

void cabPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(CAB_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void acceptRide(cab* cab, int rideType, int rideTime) {
    if (rideType == POOL_CAB) {
        assert(cab->state != onRidePoolFull);
        cab->state++;
    } else {
        assert(cab->state == waitState);
        cab->state = onRidePremier;
    }
    sleep(rideTime);
}

int usefulCab(int reqCab, int currState) {
    switch (reqCab) {
        case -1:
            return 0;
            break;
        case POOL_CAB:
            return (currState == waitState || currState == onRidePoolOne);
            break;
        case PREMIER_CAB:
            return (currState == waitState);
            break;
        default:
            assert(0);
            break;
    }
}

void endRide(cab* cab, rider* rider) {
    assert(cab->state != waitState);

    pthread_mutex_lock(&checkCab);

    totalCabsOpen++;
    if (cab->state == onRidePremier || cab->state == onRidePoolOne) {
        cab->state = waitState;
        cab->rider1 = cab->rider2 = -1;
    } else if (cab->state == onRidePoolFull) {
        cab->state = onRidePoolOne;
        cab->rider2 = -1;
    }

    int j = (rider->id + 1) % ridersCount;
    while (j != rider->id && !usefulCab(riders[j]->isWaiting, cab->state)) {
        j = (j + 1) % ridersCount;
    }

    if (j == rider->id) {
        pthread_mutex_unlock(&checkCab);
    } else {
        pthread_cond_signal(&riders[j]->cond);
        pthread_mutex_unlock(&checkCab);
    }
}

void startAndEndRide(cab* cab, rider* rider) {
    cabPrintMsg(cab->id, "taking rider %d\n", rider->id + 1);

    sleep(rider->rideTime);

    endRide(cab, rider);
}