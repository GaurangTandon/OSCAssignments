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
        pthread_mutex_lock(&updateMutex);
    check:
        for (int i = 0; i < tableCount; i++) {
            if (tables[i]->needVessel) {
                robot->biryaniVesselsRemaining--;
                robotPrintMsg(robot->id, "Serving biryani to table %d\n",
                              tables[i]);
                pthread_cond_signal(tables[i]);
                pthread_mutex_unlock(&updateMutex);
                goto success;
            }
        }
        break;
    success:
        continue;
    }

    if (robot->biryaniVesselsRemaining == 0) {
        robotPrintMsg(robot->id,
                      "finished all my vessels. Moving onto next batch\n");
        prepBiryani(robot);
    } else {
        pthread_cond_wait(&robotConditions[robot->id], &updateMutex);
        goto check;
    }
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

    prepBiryani(myrobot);

    return NULL;
}
