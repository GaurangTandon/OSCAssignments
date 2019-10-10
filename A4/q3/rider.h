#include "common.h"

typedef struct rider {
    int cabType;
    int maxWaitTime;
    int rideTime;
    int id;
} rider;

void* initRider(rider* rider);