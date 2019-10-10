#include "common.h"

typedef struct rider {
    int cabType;
    int maxWaitTime;
    int rideTime;
    int id;
    int arrivalTime;
} rider;
rider* riders[MAX_RIDERS];

void* initRider(void* rider);