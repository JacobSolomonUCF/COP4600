#ifndef main_h
#define main_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rr.h"
#include "fcfs.h"
#include "sjf.h"

//Struct for proccess data
typedef struct process{
    int numOfProcesses;
    int runTime;
    char processType[3];
    int quantum;
    
}processInfo;
processInfo process;

//Function Prototypes
void readIn();
void processType();

#endif
