#include "rr.h"
#include "main.h"

void anyArriving(struct testcase*, int);
int arriving(struct process*, int);
int nextJob(struct testcase*, int, int);
int allDone(struct testcase*);
void addWait(struct testcase*, int, int);

void rr(struct testcase testcase)
{
  printf("%d processes\nUsing Round-Robin\nQuantum %d\n\n", testcase.numOfProcesses, testcase.quantum);

  int running = 1;

  int totalTime = 0;

  int prevJobIndex = -1;
  int currJobIndex = 0;
  struct process* currJob = NULL;
  while (running)
  {
    currJob = &testcase.processes[currJobIndex];

    if (currJob->arrived && !currJob->finished)
    {
      int burst = testcase.quantum;
      if (currJob->burst <= burst)
      {
        burst = currJob->burst;
      }
      printf("Time %d: %s selected (burst %d)\n", totalTime, currJob->name, currJob->burst);
      totalTime += burst;
      currJob->burst -= burst;

      addWait(&testcase, currJobIndex, burst);
      if (currJob->burst <= 0)
      {
        currJob->finished = totalTime;
        printf("Time %d: %s finished\n", totalTime, currJob->name);
      }
    }
    else
    {

      // addWait(&testcase, currJobIndex, burst);
    }


    prevJobIndex = currJobIndex;
    currJobIndex = nextJob(&testcase, currJobIndex, testcase.numOfProcesses);
    currJob = &testcase.processes[currJobIndex];

    anyArriving(&testcase, totalTime);

    if (allDone(&testcase))
    {
      printf("Time %d: Idle\n", totalTime);
      running = 0;
      totalTime = testcase.runTime;
    }
  }

  printf("Finished at time %d\n\n", totalTime);

  int i;
  for (i = 0; i < testcase.numOfProcesses; i++)
    printf("%s wait %d turnaround %d\n", testcase.processes[i].name, testcase.processes[i].wait, testcase.processes[i].finished - testcase.processes[i].arrival);
}

void anyArriving(struct testcase* testcase, int totalTime)
{
  int i;
  for (i = 0; i < testcase->numOfProcesses; i++)
    if (arriving(&testcase->processes[i], totalTime))
    {
      testcase->processes[i].arrived = 1;
      testcase->processes[i].wait = totalTime - testcase->processes[i].arrival;
      printf("Time %d: %s arrived\n", testcase->processes[i].arrival, testcase->processes[i].name);
    }
}

int arriving(struct process* process, int totalTime)
{
  return !process->arrived && totalTime >= process->arrival;
}

int nextJob(struct testcase* testcase, int currJobIndex, int totalJobs)
{
  if (currJobIndex == totalJobs - 1)
    return 0;
  else
    return currJobIndex + 1;
}

int allDone(struct testcase* testcase)
{
  int i;
  for (i = 0; i < testcase->numOfProcesses; i++)
    if (!testcase->processes[i].finished)
      return 0;
  return 1;
}

void addWait(struct testcase* testcase, int skip, int timeAdd)
{
  int i;
  for (i = 0; i < testcase->numOfProcesses; i++)
  {
    if (i == skip)
      continue;
    if (testcase->processes[i].arrived && !testcase->processes[i].finished)
      testcase->processes[i].wait += timeAdd;
  }
}
