#include "robot.h"

void robotChefHeader(int id) {
    printTimestamp();
    printf(KGREEN "Robot chef %d\t\t" KNRM, id + 1);
}

void printMsg(char* fmt, ...) {
    char buf[1000] = {0}, buf2[1000] = {0};
    va_list argptr;
    va_start(argptr, fmt);
    sprintf(buf, fmt, argptr);

    strcat(buf2, )

        robotChefHeader();
    printf("%s%s%s", gettimeofday(), robots, );
}

void* initRobot(void* rTemp) {
    robot* myrobot = (robot*)rTemp;

    prepBiryani(myrobot, genRandomInRange(2, 5), genRandomInRange(1, 10),
                genRandomInRange(25, 50));

    return NULL;
}

void prepBiryani(robot* robot, int timePerVessel, int numOfVessels,
                 int capacityStudents) {
    printMsg("starting with biryani.");

    for (int i = 0; i < numOfVessels; i++)
        sleep(timePerVessel);

    // once all done
    biryani_ready();
}

void biryani_ready() {
}