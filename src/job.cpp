
#include "job.h"

using namespace std;

Job::Job(Process *p){
    firstProcess = p;

    stdin = 0;
    stdout = 0;
    stderr = 0;
    timeCreated = time(0);
}
    stdin = 0; //Standard value in file descriptor
    stdout = 1; //Standard value out file descriptor
    stderr = 2; //standard value err file descriptor

}
int Job::outPutFile(const char* filename) {
    int outPutFile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);

    if(outPutFile == -1) {
        perror("dup2");
        return 1;
    }

    stdout = outPutFile;
    return 0;
}
