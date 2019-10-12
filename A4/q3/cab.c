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
        assert(cab->state != oneRidePoolFull);
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
    if (cab->state == onRidePremier) {
        cab->state = waitState;
    } else {
        cab->state--;
    }

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
    totalCabsOpen--;
    pthread_mutex_unlock(&checkCab);
}

void startAndEndRide(cab* cab, rider* rider) {
    shiftCabsAround(cab);

    cabPrintMsg(cab->id, "taking rider %d\n", rider->id + 1);

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