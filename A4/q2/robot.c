#include "robot.h"
#include "table.h"

void robotPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(ROBOT_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void biryani_ready(robot* robot);

void prepBiryani(robot* robot) {
    int timeTaken = genRandomInRange(2, 5),
        numOfVessels = genRandomInRange(1, 10),
        capacityStudents = genRandomInRange(25, 50);

    robotPrintMsg(robot->id,
                  "starting with biryani preparation, takes %d seconds to "
                  "prepare %d vessels, each feeds %d students.\n",
                  timeTaken, numOfVessels, capacityStudents);

    sleep(timeTaken);

    robot->biryaniVesselsRemaining = numOfVessels;
    robot->vesselSize = capacityStudents;

    biryani_ready(robot);
}

void biryani_ready(robot* robot) {
    robotPrintMsg(robot->id,
                  "has prepared %d vessels of biryani. Waiting for vesels to "
                  "be emptied to resume cooking\n",
                  robot->biryaniVesselsRemaining);

    while (!gameOver) {
        pthread_mutex_lock(&robotMutexes[robot->id]);

        if (robot->biryaniVesselsRemaining == 0)
            break;

        pthread_mutex_unlock(&robotMutexes[robot->id]);
    }

    pthread_mutex_trylock(&robotMutexes[robot->id]);
    pthread_mutex_unlock(&robotMutexes[robot->id]);

    if (gameOver) {
        return;
    }

    robotPrintMsg(robot->id,
                  "all my vessels have been emptied. Resuming cooking now\n");
    prepBiryani(robot);
}

void* initRobot(void* rTemp) {
    robot* myrobot = (robot*)rTemp;

    robotPrintMsg(myrobot->id, "initialized\n");

    prepBiryani(myrobot);

    robotPrintMsg(myrobot->id, "has left the system\n");

    return NULL;
}
