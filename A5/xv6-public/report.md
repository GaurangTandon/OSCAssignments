# REPORT

Name Gaurang Tandon
Roll 2018101091
The sample program was run on three different scheduling algorithms and this is the result obtained for each scheduling. We ran using `time tester2` and the wtime output is the amount of the time the children took to execute.

## DEFAULT

Ticks taken to complete: 2371

Mostly because round robin is fast I guess.

## FCFS

Ticks taken to complete: 2399.

Even though the longest process was run first, the run time is surprisingly fast.

## PBS

Ticks taken to complete: 2724.

The priorities were changed using pid/2, still the PBS method is slower probably because the round robin thing for same priority people cost it a lot of context switches.

## MLFQ

Ticks taken to complete: 2346

Feedback helps I guess. All processes strive for completion and do not wait more than a set amount of time.

## MLFQ scheduling observations

On adjusting the wait times per each queue, it is observed that processes mostly toggle between the last three queues. When its wait time in the last queue exceeds the wait limit, it ages and chanages queue and executes with high priority.

## How could this be exploited by a process?

It will give up IO just before its time slice is over so that it can retain the same priority queue over and over again. Thus it's priority will never decrement.
