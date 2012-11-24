using namespace std;

#ifndef PROCESS_H
#define PROCESS_H

#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
//Program includes:


/*
 * Datastructure for a process.
 * Keeps track of the next process in a linked list.
 */

class Process {
public:
    Process(char **args);
    int markProcessStatus(pid_t pid, int status);

    char **args; //Arguments to process
    Process *nextProcess;  
 
    pid_t pid; //Process ID
    int status;
    int stopped;
    int completed;

private:

};

#endif