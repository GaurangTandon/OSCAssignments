#include "common.h"

enum cabStates { waitState, onRidePremier, onRidePoolOne, oneRidePoolFull };

typedef struct cab {
} cab;

cab* poolOneCabs[MAX_CABS];
cab* waitingCabs[MAX_CABS];
cab* poolTwoCabs[MAX_CABS];
cab* premierCabs[MAX_CABS];

void* getNewCab(cab* cab) {
}