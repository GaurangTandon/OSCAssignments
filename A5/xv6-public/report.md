# REPORT

The sample program was run on three different scheduling algorithms and this is the result obtained for each scheduling. We ran using `time tester2` and the wtime output is the amount of the time the children took to execute.

## FCFS

Ticks taken to complete: 

## PBS

Ticks taken to complete: 

## MLFQ

Ticks taken to complete: 

## MLFQ scheduling observations

On adjusting the wait times per each queue, it is observed that processes mostly toggle between the last three queues. When its wait time in the last queue exceeds the wait limit, it ages and chanages queue and executes with high priority.
