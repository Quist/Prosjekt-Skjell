using namespace std;

#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
//Program includes:


/*
 * Datastructure for a process.
 * Keeps track of the next process in a linked list.
 */

class Process{

public:
    char **args; //Arguments to process
    
    Process(char **arguments);
    
private:
    //pid_t pid; //Process ID

    //Process *nextProcess;
};
