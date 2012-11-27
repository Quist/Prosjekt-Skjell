#include "jobcontroll.h"
using namespace std;

JobControll::JobControll(){
    timeLimit = 250;
}

void JobControll::setTimeLimit(int limit){
    timeLimit = limit;
}

void JobControll::checkJobs(Job *firstJob){
    Job *j;
    for(j = firstJob; j; j = j->nextJob){
        checkJob(j);
    }
}

void JobControll::checkJob(Job *j){
    if(checkTimeLimit(j)){
        //KILL
    }
    
}

int JobControll::checkTimeLimit(Job *j){
    time_t now = time(0);
    double diffTime = difftime(now, j->timeCreated);
    if(diffTime>= timeLimit){
        return 0;
    }
    return 1;
    
}

void readStats(){
    
}

// Flytt alt av job metoder inn i Job Controll.
//Shell variabler som trengs, kan bli globale.
