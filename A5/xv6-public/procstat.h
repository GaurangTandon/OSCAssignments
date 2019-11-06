// statistics for each process, from assignment
#define PQ_COUNT 5
struct proc_stat {
    int pid;              // PID of each process
    int runtime;          // Use suitable unit of time
    int num_run;          // number of time the process is executed
    int allotedQ[2];      // current assigned queue and position inside it
    int ticks[PQ_COUNT];  // number of ticks each process has received at each
    // of the 5 priority queue
    int actualTicks[PQ_COUNT];  // ticks keeps getting resetted everytime it is
                                // pushed to a new queue
};