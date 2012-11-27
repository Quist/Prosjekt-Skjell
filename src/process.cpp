
#include "process.h"

using namespace std;


Process::Process(char **args){
    this->args = args;
    next = NULL;
}

int Process::markProcessStatus(pid_t pid, int status) {
    if (this->pid == pid) {
        this->status = status;

        if (WIFSTOPPED(status)) {
            stopped = 1;
        } else {
            completed = 1;
            if (WIFSIGNALED(status)) {
                std::cout << "ERROR: Process terminated by signal\n";
            }
            return 0;
        }
    }
    return -1;  //Called with wrong pid
}
