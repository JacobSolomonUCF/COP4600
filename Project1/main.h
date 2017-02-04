#ifndef main_h
#define main_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rr.h"
#include "fcfs.h"
#include "sjf.h"

//Struct for proccess data
struct process {
  char* name;
  int arrival;
  int burst;
}

struct testcase {
    int numOfProcesses;
    int runTime;
    char processType[4];
    int quantum;
    process* processes;
} testcase;


//Function Prototypes
void readIn();
void processType();

#endif
