using namespace std;
#ifndef JOBCONTROLL_H
#define JOBCONTROLL_H

#include <ctime>

#include "job.h"
#include "process.h"


class JobControll {
public:
    JobControll();
    void checkJobs(Job *firstJob);


private:
    void checkJob(Job *j);
    void setTimeLimit(int limit);
    int checkTimeLimit(Job *j);
    int timeLimit;
    int memTimeLimit;
    int memLimit;
    int cpuTimeLimit;
    int cpuLimit;
};

#endif