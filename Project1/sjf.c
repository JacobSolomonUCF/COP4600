#include "sjf.h"
#include "main.h"

int lowest = -1;
int findL(struct testcase sjf);
void sjf(struct testcase sjf){
  int curpos = -1;
  int time;
  int inuse = 0;
  int i,j;

  printf("%d processes\nUsing Shortest Job First (Pre)\n\n",sjf.numOfProcesses );

  for(time = 0; time < sjf.runTime; time++)//run through the total run time
  {
    for(i = 0; i < sjf.numOfProcesses;i++)//run through the number of processes
    {
      if(sjf.processes[i].arrival == time)
      {
        printf("Time %d: %s arrived\n",time, sjf.processes[i].name);
        sjf.processes[i].arrived = 1;
        sjf.processes[i].turnaround = sjf.processes[i].burst;

      }
    }
    if(sjf.processes[curpos].burst == 0 && curpos != -1){
      printf("Time %d: %s finished\n",time, sjf.processes[curpos].name);
      sjf.processes[curpos].arrived = -1;
      sjf.processes[curpos].finished = time;
      lowest = -1;
      inuse = 0;
    }
    if(inuse == 1){
      int next = curpos;
      next = findL(sjf);
      if(next != curpos){
        curpos = next;
        printf("Time %d: %s selected (burst %d)\n", time, sjf.processes[curpos].name,sjf.processes[curpos].burst);
      }

    }
    if(inuse == 0)
    {
      int flag = 0;
      for(j = 0; j < sjf.numOfProcesses;j++){
        if(sjf.processes[j].arrived == 1){
          flag = 1;
        }
      }
      if(flag == 1){
        curpos = findL(sjf);
        printf("Time %d: %s selected (burst %d)\n", time, sjf.processes[curpos].name,sjf.processes[curpos].burst);
        inuse = 1;
      }else{
        printf("Time %d: IDLE\n",time );
      }

    }


    sjf.processes[curpos].burst--;
  }
  printf("Finished at time %d\n\n", time);
  for(i = 0; i<sjf.numOfProcesses; i++){
    int TA = sjf.processes[i].finished - sjf.processes[i].arrival;
    int Wait = TA - sjf.processes[i].turnaround;
    printf("%s wait %d turnaround %d \n",sjf.processes[i].name,Wait,TA );
  }

}
int findL(struct testcase sjf){
  int i;
  for(i = 0; i<sjf.numOfProcesses; i++){
    if(sjf.processes[i].arrived == 1 && lowest == -1){
      lowest = i;
    }
    if(sjf.processes[i].arrived == 1){
      if(sjf.processes[i].burst < sjf.processes[lowest].burst){
        lowest = i;
      }
    }
  }
  return lowest;
}
