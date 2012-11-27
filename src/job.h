
using namespace std;

#ifndef JOB_H
#define JOB_H

#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>


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

    int outPutFile(const char* filename);
    
    
private:    

};

#endif
