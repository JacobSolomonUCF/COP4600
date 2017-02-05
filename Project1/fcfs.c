#include "fcfs.h"
#include "main.h"

void fcfs(struct testcase fcfs){
  int position = 0;
  int index = 0;
  int inuse = 0;
  int time = 0, i = 0;
  int q[fcfs.numOfProcesses];

  printf("%d processes\nUsing First Come First Served\n\n",fcfs.numOfProcesses);

  for(time = 0; time <= fcfs.runTime;time++){

    for(i = 0; i<fcfs.numOfProcesses; i++){
      if(fcfs.processes[i].arrival == time){
        printf("Time %d: %s arrived\n", time,fcfs.processes[i].name );
        fcfs.processes[i].arrived = time;
        q[index] = i;
        index++;
      }

      if(inuse == 1){
        if(fcfs.processes[q[position]].burst == 0){
          printf("Time %d: %s finished\n",time,fcfs.processes[q[position]].name );
          fcfs.processes[q[position]].finished = (time- fcfs.processes[q[position]].arrival);
          position++;
          inuse = 0;
          if(position == fcfs.numOfProcesses){
            break;
          }
        }
      }
      if(inuse == 0){
        printf("Time %d: %s selected (burst %d)\n",time,fcfs.processes[q[position]].name, fcfs.processes[q[position]].burst);
        fcfs.processes[q[position]].wait = time - fcfs.processes[q[position]].arrived;
        inuse = 1;
      }

    }

    fcfs.processes[q[position]].burst--;

  }
  printf("\n" );

  for(i = 0; i<fcfs.numOfProcesses; i++){
      printf("%s wait %d turnaround %d\n",fcfs.processes[i].name, fcfs.processes[i].wait, fcfs.processes[i].finished );
    }




}
