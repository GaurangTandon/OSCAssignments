#ifndef cabber
#define cabber
#include "common.h"

// the order matters here, see acceptRide function in cab.c
enum cabStates { waitState, onRidePoolOne, onRidePoolFull, onRidePremier };

// keep track of usable cabs
cab** cabs;

void* initCab(void* cab);

cab* getFreeCab(int cabType);

void startAndEndRide(cab* cab, rider* rider);
void cabPrintMsg(int id, char* fmt, ...);
#endif