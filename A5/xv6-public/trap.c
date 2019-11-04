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
                updateStats();
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

    int queueIdx = currp ? currp->allotedQ[0] - 1 : 0;

    if (currp && currp->state == RUNNING && tf->trapno == T_IRQ0 + IRQ_TIMER) {
        if (ifMeraProc(currp))
            cprintf("[TRAP] %d %s\n", currp->pid, currp->name);
        // do a round robin, my time slice is over
        if (ticks % (1 << queueIdx) == 0) {
            popFront(queueIdx);

            if (queueIdx == PQ_COUNT - 1) {
                currp->allotedQ[1] = prioQSize[queueIdx];
                pushBack(queueIdx, currp);
            } else {
                currp->allotedQ[0]++;
                currp->allotedQ[1] = prioQSize[queueIdx + 1];
                pushBack(queueIdx + 1, currp);
            }

            // this yield is creating a pagefault, need to figure out when to
            // call this
            yield();
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
