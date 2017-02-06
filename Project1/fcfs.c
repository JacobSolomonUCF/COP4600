#include "fcfs.h"
#include "main.h"

void fcfs(struct testcase fcfs){
  FILE * out = fopen("processes.out","w");
  int flag = 0,position = 0, index = 0 ;
  int time = 0, i = 0, inuse = 0;
  int q[fcfs.numOfProcesses];

  //Header
  fprintf(out, "%d processes\nUsing First Come First Served\n\n",fcfs.numOfProcesses);

  //Core Loop
  for(time = 0; time <= fcfs.runTime;time++){
    for(i = 0; i<fcfs.numOfProcesses; i++){ //Checking for arrival
      if(fcfs.processes[i].arrival == time){
        fprintf(out,"Time %d: %s arrived\n", time,fcfs.processes[i].name );
        fcfs.processes[i].arrived = time;
        q[index] = i;
        index++;
      }
    }
    if(inuse == 1){ //If a process is running
      int firstTime = 0;
      if(fcfs.processes[q[position]].burst == 0){

        fprintf(out,"Time %d: %s finished\n",time,fcfs.processes[q[position]].name);
        fcfs.processes[q[position]].finished = (time- fcfs.processes[q[position]].arrival);
        flag++;
        position++;
        inuse = 0;

        if(position == fcfs.numOfProcesses){ //If there are no more processes to run
          firstTime = 1;
          while(time < fcfs.runTime){
            if(firstTime != 1){
              fprintf(out,"Time %d: IDLE\n",time );
              time++;
            }else{
              time++;
              firstTime = 0;
            }
          }
          fprintf(out,"Finished at time %d\n\n", time);
          break;
        }
      }
    }

    if(inuse == 0){ //If the processor is free
        fprintf(out,"Time %d: %s selected (burst %d)\n",time,fcfs.processes[q[position]].name, fcfs.processes[q[position]].burst);
        fcfs.processes[q[position]].wait = time - fcfs.processes[q[position]].arrived;
        inuse = 1;
    }

    fcfs.processes[q[position]].burst--;
  }

  //Printing wait & Turn around time
  for(i = 0; i<fcfs.numOfProcesses; i++){
      fprintf(out,"%s wait %d turnaround %d\n",fcfs.processes[i].name, fcfs.processes[i].wait, fcfs.processes[i].finished );
    }



fclose(out);
}
