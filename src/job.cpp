
#include "job.h"

using namespace std;

Job::Job(Process *p, string n){
    firstProcess = p;
    name = n;

    stdin = 0; //Standard value in file descriptor
    stdout = 1; //Standard value out file descriptor
    stderr = 2; //standard value err file descriptor
    timeCreated = time(0);
}

int Job::outPutFile(const char* filename) {
    int outPutFile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);

    if(outPutFile == -1) {
        perror("open");
        return 0;
    }

        if(dup2(outPutFile, 1) == -1) {
        perror("dup2");
        return 0;
        }

    stdout = outPutFile;
    return 1;
}

int Job::inPutFile(const char* filename) {
    int inPutFile = open(filename, O_RDONLY, 0666);

    if(inPutFile == -1) {
        perror("dup2");
        return 0;
    }

    if(dup2(inPutFile, 0) == -1) {
        perror("dup2");
        return 0;
    }

    stdin = inPutFile;
    return 1;
}

int Job::errPutFile(const char* filename) {
    int errPutFile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);

    if(errPutFile == -1) {
        perror("open");
        return 0;
    }

        if(dup2(errPutFile, 2) == -1) {
        perror("dup2");
        return 0;
    }

    stderr = errPutFile;
    return 1;
}

