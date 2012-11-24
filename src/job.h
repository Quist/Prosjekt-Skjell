
using namespace std;

#include <cstdlib>

//Program includes:
#include "process.cpp"

/* Structure for a job.
 * Processes in the job are stored in a linked list.
 */
class Job {
public:
    int stdin, stdout, stderr; /* standard i/o channels */
    Process *firstProcess;
    Job *nextJob;
    pid_t pgid;
    struct termios tmodes;      /* saved terminal modes */
    
    
private:
    

};
