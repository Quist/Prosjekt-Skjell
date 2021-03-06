
using namespace std;

#ifndef JOB_H
#define JOB_H

#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <ctime>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string>

//Program includes:
#include "process.h"

/* Structure for a job.
 * Processes in the job are stored in a linked list.
 */
class Job {
public:
    Job(Process *p, string name);
    string name;
    
    //standard i/o channels:
    int stdin;
    int stdout;
    int stderr; 
    
    Process *firstProcess;
    Job *nextJob;
    pid_t pgid;
    struct termios tmodes; //Saved terminal mode
    time_t timeCreated;
    
    int outPutFile(const char* filename);
    int inPutFile(const char* filename);
    int errPutFile(const char* filename);
    
    
private:    

};

#endif
