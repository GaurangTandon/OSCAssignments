#include "robot.h"
#include "table.h"

void robotPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(ROBOT_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void biryani_ready(robot* robot) {
    while (robot->biryaniVesselsRemaining) {
        for (int i = 0; i < tableCount; i++) {
            pthread_mutex_lock(&tableMutexes[i]);

            if (tables[i]->biryaniAmountRemaining == 0) {
                robot->biryaniVesselsRemaining--;
                pthread_cond_signal(tables[i]);
                pthread_mutex_unlock(&tableMutexes[i]);
                goto success;
            }
        }
        break;
    success:
        continue;
    }
}

void prepBiryani(robot* robot, int timeTaken, int numOfVessels,
                 int capacityStudents) {
    robotPrintMsg(robot->id,
                  "starting with biryani prepartion, takes %d seconds to "
                  "prepare %d vessels, each feeds %d students.",
                  timeTaken, numOfVessels, capacityStudents);

    sleep(timeTaken);
    robot->biryaniVesselsRemaining = numOfVessels;

    biryani_ready(robot);
}

void* initRobot(void* rTemp) {
    robot* myrobot = (robot*)rTemp;

    prepBiryani(myrobot, genRandomInRange(2, 5), genRandomInRange(1, 10),
                genRandomInRange(25, 50));

    return NULL;
}
