#include "robot.h"

void robotPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(ROBOT_TYPE, id, fmt, argptr);
}

void biryani_ready() {
}

void prepBiryani(robot* robot, int timePerVessel, int numOfVessels,
                 int capacityStudents) {
    robotPrintMsg(robot->id, "starting with biryani.");

    for (int i = 0; i < numOfVessels; i++)
        sleep(timePerVessel);

    // once all done
    biryani_ready();
}

void* initRobot(void* rTemp) {
    robot* myrobot = (robot*)rTemp;

    prepBiryani(myrobot, genRandomInRange(2, 5), genRandomInRange(1, 10),
                genRandomInRange(25, 50));

    return NULL;
}
