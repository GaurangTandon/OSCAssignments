#include "robot.h"

void robotPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(ROBOT_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void biryani_ready() {
}

void prepBiryani(robot* robot, int timeTaken, int numOfVessels,
                 int capacityStudents) {
    robotPrintMsg(robot->id,
                  "starting with biryani prepartion, takes %d seconds to "
                  "prepare %d vessels, each feeds %d students.",
                  timeTaken, numOfVessels, capacityStudents);

    sleep(timeTaken);

    // once all done
    biryani_ready();
}

void* initRobot(void* rTemp) {
    robot* myrobot = (robot*)rTemp;

    prepBiryani(myrobot, genRandomInRange(2, 5), genRandomInRange(1, 10),
                genRandomInRange(25, 50));

    return NULL;
}
