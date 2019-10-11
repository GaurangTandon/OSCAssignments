#include "robot.h"

void printMsg(int id, char* fmt, ...) {
    char* buf = (char*)calloc(sizeof(char) * 1000, 1);
    char buf2[1000] = {0};
    va_list argptr;
    va_start(argptr, fmt);
    sprintf(buf, fmt, argptr);

    strcat(buf2, getHeader(ROBOT_TYPE, id));
    strcat(buf2, buf);

    printf("%s\n", buf2);
}

void biryani_ready() {
}

void prepBiryani(robot* robot, int timePerVessel, int numOfVessels,
                 int capacityStudents) {
    printMsg(robot->id, "starting with biryani.");

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
