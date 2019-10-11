#include "robot.h"
#include "table.h"

void robotPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(ROBOT_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void biryani_ready(robot* robot) {
    while (1) {
        pthread_mutex_lock(&robotMutexes[robot->id]);

        if (robot->biryaniVesselsRemaining == 0) {
            break;
        }

        pthread_mutex_unlock(&robotMutexes[robot->id]);
    }

    robotPrintMsg(robot->id,
                  "finished all my vessels. Moving onto next batch\n");
    prepBiryani(robot);
}

void prepBiryani(robot* robot) {
    int timeTaken = genRandomInRange(2, 5),
        numOfVessels = genRandomInRange(1, 10),
        capacityStudents = genRandomInRange(25, 50);

    robotPrintMsg(robot->id,
                  "starting with biryani prepartion, takes %d seconds to "
                  "prepare %d vessels, each feeds %d students.\n",
                  timeTaken, numOfVessels, capacityStudents);

    sleep(timeTaken);
    robot->biryaniVesselsRemaining = numOfVessels;
    robot->vesselSize = capacityStudents;

    biryani_ready(robot);
}

void* initRobot(void* rTemp) {
    robot* myrobot = (robot*)rTemp;

    robotPrintMsg("initialized\n");

    prepBiryani(myrobot);

    return NULL;
}
