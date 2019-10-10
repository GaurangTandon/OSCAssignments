#include "common.h"

// the order matters here, see acceptRide function in cab.c
enum cabStates { waitState, onRidePoolOne, oneRidePoolFull, onRidePremier };

typedef struct cab {
    int state;
} cab;

cab* poolOneCabs[MAX_CABS];
cab* waitingCabs[MAX_CABS];
cab* poolTwoCabs[MAX_CABS];
cab* premierCabs[MAX_CABS];

void* initCab(cab* cab) {
}