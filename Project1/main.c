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
    while(strcmp(tmp, "end") != 0){
        fscanf(ifp, "%s", tmp);
        if (strcmp(tmp, "processcount") == 0)
            fscanf(ifp, "%d", &process.numOfProcesses);
        if(strcmp(tmp, "runfor") == 0)
            fscanf(ifp, "%d", &process.runTime);
        if(strcmp(tmp, "use") == 0){
            fscanf(ifp, "%s", process.processType);
            if(strcmp(process.processType, "rr")== 0 ){
                fscanf(ifp, "%s", tmp);
                if(strcmp(tmp,"quantum") == 0)
                    fscanf(ifp, "%d", &process.quantum);

            }
            fclose(ifp);
            return;
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
