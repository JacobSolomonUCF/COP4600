#include "main.h"

int main() {

    //Test for linking files can be removed.
    rr();
    sjf();
    fcfs();


    readIn();
    processType();


    //TESTING OUTPUT
    printf("Num Of Proccesses %d\nRun Time %d\nProcess Type %s\nQuantum %d\n\n", process.numOfProcesses,process.runTime,process.processType,process.quantum);

    return 0;
}

void processType(){
    char *tmp = malloc(128);
    FILE * ifp = fopen("clean.out", "r");
    int index = 0;
    while(strcmp(tmp, "end") != 0){
        fscanf(ifp, "%s", tmp);
        if (strcmp(tmp, "processcount") == 0) {
            fscanf(ifp, "%d", &testcase.numOfProcesses);
            testcase.processes = malloc(testcase.numOfProcesses * sizeof(process))
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
            fclose(ifp);
            return;
        }
        if (strcmp(tmp, "process") == 0) {
          fscanf(in, "%s", tmp);
          fscanf(in, "%s", tmp);
          strcpy(fc[index].processID, tmp);
          fscanf(in, "%s", tmp);
          if(strcmp(tmp, "arrival") == 0){
            fscanf(in, "%d", &fc[index].arrivalTime);

          }
          fscanf(in, "%s", tmp);
          if(strcmp(tmp, "burst") == 0){
            fscanf(in, "%d", &fc[index].burstTime);
            totalTime = totalTime + fc[index].burstTime;
          }
          index++;
        }
      }
    }
    fclose(ifp);
    return;
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
