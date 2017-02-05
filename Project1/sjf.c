#include "sjf.h"
#include "main.h"

struct arrivedProcesses {
  char  name[4];
  int   index;
  int   burstTime;

};

struct arrivedProcesses* arrivedProcesses;

void sjf(struct testcase sjf){
  int time;
  int tempProcCount = 0;

  int i, p[sjf.numOfProcesses];//q is an order of arrived processes

  for(time = 0; time < sjf.runTime; time++)//run through the total run time
  {
    for(i = 0; i < sjf.numOfProcesses;i++)//run through the number of processes
    {
      if(sjf.processes[i].arrival == time)
      {
        printf("Process %s has arrived!", sjf.processes[i].name);
        sjf.processes[i].arrived = 1;

        strcpy(arrivedProcesses[tempProcCount].name, sjf.processes[i].name);
        arrivedProcesses[tempProcCount].burstTime = sjf.processes[i].burst;


      }
    }
  }
  printf("TEST OF sjf\n"); //Test for linking files
}
