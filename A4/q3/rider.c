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

void* bookCab(rider* rider) {
}

void makePayment() {
}