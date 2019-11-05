#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

void tvinit(void) {
    int i;

    for (i = 0; i < 256; i++)
        SETGATE(idt[i], 0, SEG_KCODE << 3, vectors[i], 0);
    SETGATE(idt[T_SYSCALL], 1, SEG_KCODE << 3, vectors[T_SYSCALL], DPL_USER);

    initlock(&tickslock, "time");
}

void idtinit(void) {
    lidt(idt, sizeof(idt));
}

// PAGEBREAK: 41
// anywhere before dereferencing myproc(), check if it is not null
void trap(struct trapframe *tf) {
    if (tf->trapno == T_SYSCALL) {
        if (!myproc() || myproc()->killed)
            exit();
        myproc()->tf = tf;
        syscall();
        if (myproc()->killed)
            exit();
        return;
    }

    switch (tf->trapno) {
        case T_IRQ0 + IRQ_TIMER:
            if (cpuid() == 0) {
                acquire(&tickslock);
                ticks++;
                if (myproc() && myproc()->state == RUNNING) {
                    myproc()->rtime++;
                }
                wakeup(&ticks);
                release(&tickslock);
            }
            lapiceoi();
            break;
        case T_IRQ0 + IRQ_IDE:
            ideintr();
            lapiceoi();
            break;
        case T_IRQ0 + IRQ_IDE + 1:
            // Bochs generates spurious IDE1 interrupts.
            break;
        case T_IRQ0 + IRQ_KBD:
            kbdintr();
            lapiceoi();
            break;
        case T_IRQ0 + IRQ_COM1:
            uartintr();
            lapiceoi();
            break;
        case T_IRQ0 + 7:
        case T_IRQ0 + IRQ_SPURIOUS:
            cprintf("cpu%d: spurious interrupt at %x:%x\n", cpuid(), tf->cs,
                    tf->eip);
            lapiceoi();
            break;

            // PAGEBREAK: 13
        default:
            if (myproc() == 0 || (tf->cs & 3) == 0) {
                if (myproc() == 0)
                    cprintf("Pagefault due to empty process.\n");
                // In kernel, it must be our mistake.
                cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
                        tf->trapno, cpuid(), tf->eip, rcr2());
                panic("trap");
            }
            // In user space, assume process misbehaved.
            cprintf(
                "pid %d %s: trap %d err %d on cpu %d "
                "eip 0x%x addr 0x%x--kill proc\n",
                myproc()->pid, myproc()->name, tf->trapno, tf->err, cpuid(),
                tf->eip, rcr2());
            myproc()->killed = 1;
    }

    // Force process exit if it has been killed and is in user space.
    // (If it is still executing in the kernel, let it keep running
    // until it gets to the regular system call return.)
    if (myproc() && myproc()->killed && (tf->cs & 3) == DPL_USER)
        exit();
#ifdef FCFS
        // no yielding here, let the previously running process prevail
#else
#ifdef MLFQ
    struct proc* currp = myproc();

    if (currp && tf->trapno == T_IRQ0 + IRQ_TIMER) {
        int queueIdx = currp->allotedQ[0] - 1;

        if (queueIdx < 0) {
            cprintf("%d %d\n", queueIdx, currp->pid);
            panic("Invalid queue allotment");
        }

        switch (currp->state) {
            case RUNNING:
                // do a round robin, my time slice is over
                if (currp->stat->ticks[queueIdx] % (1 << queueIdx) == 0) {
                    decPrio(queueIdx);
                    cprintf("Proc %d preempted\n", currp->pid);
                    currp->stat->ticks[currp->allotedQ[0]] = 0;
                    yield();
                }
                break;
            case RUNNABLE:
                if (currp->stat->ticks[queueIdx] >= WAIT_LIMIT) {
                    currp->stat->ticks[queueIdx] = 0;
                    cprintf("Process %d aged\n", currp->pid);
                    incPrio(queueIdx, currp->allotedQ[1]);
                    currp->stat->ticks[currp->allotedQ[0]] = 0;
                }
                break;
            case UNUSED:
            case EMBRYO:
            case SLEEPING:
            case ZOMBIE:
                break;
        }
    }

#else
#ifdef PBS
    if (myproc() && myproc()->state == RUNNING &&
        tf->trapno == T_IRQ0 + IRQ_TIMER) {
        if (timeToPreempt(myproc()->priority)) {
            yield();
        }
    }
#else
    // abhi ke liye pbs mein bhi context switches ho rahe hain
    // Force process to give up CPU on clock tick.
    // If interrupts were on while locks held, would need to check nlock.
    if (myproc() && myproc()->state == RUNNING &&
        tf->trapno == T_IRQ0 + IRQ_TIMER)
        yield();
#endif
#endif
#endif

    // Check if the process has been killed since we yielded
    if (myproc() && myproc()->killed && (tf->cs & 3) == DPL_USER)
        exit();
}
