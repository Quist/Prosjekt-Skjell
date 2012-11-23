using namespace std;

#include <cstdlib>

//Program includes:


/*
 * Datastructure for a process.
 * Keeps track of the next process in a linked list.
 */

class Process{
    
 public:

private:
    pid_t pid;  //Process ID
    char **args; //Arguments to process
    Process *nextProcess;
};
