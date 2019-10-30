// #include "types.h"
// #include "user.h"

// #define CPU_BOUND 0
// #define SCPU_BOUND 1
// #define IO_BOUND 2

// int main(int argc, char* argv[]) {
//     if (argc != 2) {
//         printf(1, "Usage: simulate <n>\n");
//         exit();
//     }

//     int n = atoi(argv[1]);
//     const int LARGE_INT = 1000000;
//     const int SMALL_INT = 100;
//     int procsCount = 3 * n;

//     int sums[3][3];
//     for (int i = 0; i < 3; i++)
//         for (int j = 0; j < 3; j++)
//             sums[i][j] = 0;

//     for (int i = 0; i < procsCount; i++) {
//         int pid = fork();

//         if (pid == 0) {
//             int rem = getpid() % 3;
//             switch (rem) {
//                 case CPU_BOUND:
//                     for (int k = 0; k < SMALL_INT; k++) {
//                         for (int j = 0; j < LARGE_INT; j++) {
//                         }
//                     }
//                     break;
//                 case SCPU_BOUND:
//                     for (int k = 0; k < SMALL_INT; k++) {
//                         for (int j = 0; j < LARGE_INT; j++) {
//                         }
//                         yield();
//                     }
//                     break;
//                 case IO_BOUND:
//                     for (int k = 0; k < SMALL_INT; k++) {
//                         sleep(1);
//                     }
//                     break;
//             }
//             exit();
//         }
//     }

//     for (int i = 0; i < procsCount; i++) {
//         int retime, rutime, stime, pid = wait2(&retime, &rutime, &stime),
//                                    rem = pid % 3;

//         switch (rem) {
//             case CPU_BOUND:
//                 printf(1,
//                        "CPU-bound, pid: %d, ready: %d, running: %d, sleeping:
//                        "
//                        "%d, turnaround: %d\n",
//                        pid, retime, rutime, stime, retime + rutime + stime);
//                 sums[0][0] += retime;
//                 sums[0][1] += rutime;
//                 sums[0][2] += stime;
//                 break;
//             case SCPU_BOUND:
//                 printf(1,
//                        "CPU-S bound, pid: %d, ready: %d, running: %d, "
//                        "sleeping: %d, turnaround: %d\n",
//                        pid, retime, rutime, stime, retime + rutime + stime);
//                 sums[1][0] += retime;
//                 sums[1][1] += rutime;
//                 sums[1][2] += stime;
//                 break;
//             case IO_BOUND
//                 printf(1,
//                        "I/O bound, pid: %d, ready: %d, running: %d, sleeping:
//                        "
//                        "%d, turnaround: %d\n",
//                        pid, retime, rutime, stime, retime + rutime + stime);
//                 sums[2][0] += retime;
//                 sums[2][1] += rutime;
//                 sums[2][2] += stime;
//                 break;
//         }
//     }
//     for (int i = 0; i < 3; i++)
//         for (int j = 0; j < 3; j++)
//             sums[i][j] /= n;

//     printf(
//         1,
//         "\n\nCPU bound:\nAverage ready time: %ld\nAverage running time: "
//         "%ld\nAverage sleeping time: %ld\nAverage turnaround time:
//         %ld\n\n\n", sums[0][0], sums[0][1], sums[0][2], sums[0][0] +
//         sums[0][1] + sums[0][2]);
//     printf(
//         1,
//         "CPU-S bound:\nAverage ready time: %d\nAverage running time: "
//         "%d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n",
//         sums[1][0], sums[1][1], sums[1][2],
//         sums[1][0] + sums[1][1] + sums[1][2]);
//     printf(
//         1,
//         "I/O bound:\nAverage ready time: %d\nAverage running time: "
//         "%d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n",
//         sums[2][0], sums[2][1], sums[2][2],
//         sums[2][0] + sums[2][1] + sums[2][2]);
// }