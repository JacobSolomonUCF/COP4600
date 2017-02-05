#include "main.h"
#include "rr.h"
#include "sjf.h"
#include "fcfs.h"

int main() {

    readIn();
    processType();

    if(strcmp(testcase.processType,"rr") == 0){
        rr(testcase);
    }
    if(strcmp(testcase.processType,"sjf") == 0){
        sjf(testcase);
    }
    if(strcmp(testcase.processType,"fcfs") == 0){
        fcfs(testcase);
    }

    return 0;
}

void processType(){
    char *tmp = malloc(128);
    FILE * ifp = fopen("clean.out", "r");
    int index = 0, totalTime = 0;


    while(strcmp(tmp, "end") != 0){
        fscanf(ifp, "%s", tmp);
        if (strcmp(tmp, "processcount") == 0) {
            fscanf(ifp, "%d", &testcase.numOfProcesses);
            testcase.processes = malloc(testcase.numOfProcesses * sizeof(struct process));
        }
        if (strcmp(tmp, "runfor") == 0)
            fscanf(ifp, "%d", &testcase.runTime);
        if (strcmp(tmp, "use") == 0) {
            fscanf(ifp, "%s", testcase.processType);
            if(strcmp(testcase.processType, "rr")== 0 ){
                fscanf(ifp, "%s", tmp);
                if(strcmp(tmp,"quantum") == 0)
                    fscanf(ifp, "%d", &testcase.quantum);
            }

        }
        if (strcmp(tmp, "process") == 0) {
          fscanf(ifp, "%s", tmp);
          fscanf(ifp, "%s", tmp);
          strcpy(testcase.processes[index].name, tmp);
          fscanf(ifp, "%s", tmp);
          if(strcmp(tmp, "arrival") == 0){
            fscanf(ifp, "%d", &testcase.processes[index].arrival);

          }
          fscanf(ifp, "%s", tmp);
          if(strcmp(tmp, "burst") == 0){
            fscanf(ifp, "%d", &testcase.processes[index].burst);
            totalTime = totalTime + testcase.processes[index].burst;
          }
          testcase.processes[index].finished = 0;
          testcase.processes[index].arrived = 0;
          testcase.processes[index].wait = 0;
          testcase.processes[index].turnaround = 0;
          index++;
        }
      }

    fclose(ifp);
    }


void readIn(){
    FILE * ifp = fopen("processes.in", "r");
    FILE * cleanedOutput = fopen("clean.out", "w");

    char ch = getc(ifp);
    while(ch != EOF){
        if(ch == '#'){
            while(ch != '\n')
                ch = getc(ifp);
        }
        fprintf(cleanedOutput, "%c", ch);
        ch = getc(ifp);
    }


    fclose(cleanedOutput);
    fclose(ifp);
}
