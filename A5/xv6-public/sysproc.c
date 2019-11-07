#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void) {
    return fork();
}

int sys_exit(void) {
    if (!myproc())
        panic("Exitinng dead process");
    if (!PLOT && DEBUG)
        cprintf("exit %d\n", myproc()->pid);
#ifdef FCFS
    cprintf("ctime %d\n", myproc()->ctime);
#endif
    exit();
    return 0;  // not reached
}

int sys_wait(void) {
    return wait();
}

int sys_waitx(void) {
    int *wtime, *rtime;

    if (argptr(0, (void *)&wtime, 4) < 0 || argptr(1, (void *)&rtime, 4) < 0)
        return -1;

    return waitx(wtime, rtime);
}

int sys_kill(void) {
    int pid;

    if (argint(0, &pid) < 0)
        return -1;
    return kill(pid);
}

int sys_getpid(void) {
    return myproc()->pid;
}

int sys_getpinfo(void) {
    struct proc_stat *ps;
    int pid;

    if (argptr(0, (void *)&ps, sizeof(ps)) < 0 || argint(1, &pid) < 0)
        return -1;

    return getpinfo(ps, pid);
}

int sys_sbrk(void) {
    int addr;
    int n;

    if (argint(0, &n) < 0)
        return -1;
    addr = myproc()->sz;
    if (growproc(n) < 0)
        return -1;
    return addr;
}

int sys_sleep(void) {
    int n;
    uint ticks0;

    if (argint(0, &n) < 0)
        return -1;
    acquire(&tickslock);
    ticks0 = ticks;
    while (ticks - ticks0 < n) {
        if (myproc()->killed) {
            release(&tickslock);
            return -1;
        }
        sleep(&ticks, &tickslock);
    }
    release(&tickslock);
    return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void) {
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);
    return xticks;
}

// change priority of current running process to newPriority
// and return old priority
int sys_set_priority(void) {
    int newPriority;

    if (argint(0, &newPriority) < 0)
        return -1;

    return set_prio(newPriority);
}