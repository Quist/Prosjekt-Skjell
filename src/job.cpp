
#include "job.h"

using namespace std;

Job::Job(Process *p){
    firstProcess = p;

    stdin = 0;
    stdout = 0;
    stderr = 0;
}
