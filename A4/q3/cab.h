#ifndef cabber
#define cabber
#include "common.h"

// the order matters here, see acceptRide function in cab.c
enum cabStates { waitState, onRidePoolOne, oneRidePoolFull, onRidePremier };

// keep track of usable cabs
cab** poolOneCabs;
cab** waitingCabs;

void* initCab(void* cab);

cab* getFreeCab(int cabType);

void startAndEndRide(cab* cab, rider* rider);
void cabPrintMsg(int id, char* fmt, ...);
#endif