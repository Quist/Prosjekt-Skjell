
using namespace std;

#ifndef JOB_H
#define JOB_H

#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <ctime>
//Program includes:
#include "process.h"

/* Structure for a job.
 * Processes in the job are stored in a linked list.
 */
class Job {
public:
    Job(Process *p);
    int stdin;
    int stdout;
    int stderr; /* standard i/o channels */
    
    Process *firstProcess;
    Job *nextJob;
    pid_t pgid;
    struct termios tmodes;      /* saved terminal modes */
    time_t timeCreated;
    
    
    
private:    

};

#endif
