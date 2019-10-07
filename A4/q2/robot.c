#include "common.h"

void prepBiryani(int timePerVessel, int numOfVessels, int capacityStudents) {
    for (int i = 0; i < numOfVessels; i++)
        sleep(timePerVessel);

    // once all done
    biryani_ready();
}

void biryani_ready() {
}