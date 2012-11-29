
using namespace std;

#ifndef SHELL_H
#define SHELL_H

#include <cstdlib>
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <list>
#include <termios.h>
#include <sys/stat.h>
#include <sys/wait.h>

//Program includes:
#include "history.h"
#include "process.h"
#include "job.h"

class Shell {
    public:
        void orderLoop();
        Shell();

    private:
        History his;
        pid_t shellPGID;
        struct termios shellMode;
        int foregroundTerminal;
        int interactive;
        int jobCount;
        Job *firstJob;
        list<string> commands;
        char currentPath[1024];
        char currentData[1024];


        void test(string cmd);
        void prepareJob(string cmd, int foreground);
        void testPiping();

        void initShell();
        void setStartPath();
        void updateCurrentPath(char newPath[]);
        void checkCommand(string userInput);
        void execForLoop(list<string> command, int start, int end, int increment, int lessThan);
        void handleUserInput(string userInput);

        void launchProcess(Process *p, pid_t pgid, int infile, int outfile,
                int errfile, int foreground);

        void launchJob(Job *j, int foreground);
        void putJobInForeground(Job *j, int cont);
        void putJobInBackground(Job *j, int cont);
        int markProcessStatus(pid_t pid, int status);
        void waitForJob(Job *j);
        int jobIsStopped(Job *j);
        int jobIsCompleted(Job *j);
        void addJob(Job *j);
        void showJobs();
        void killJob();
        void bringJobToForeground(int pid);
        Job* findJob(int pgid);
        void killJob(int pgid);
        void removeJob(int pgid);

        void writeToFile(string fileName, list<string> l);
        void readFile(string fileName);
        bool dirChecker(char dir[]);
        int setPathOrData(string userInput);
        int findCommandNum(string userInput);
        int setCPUorMEM(string userInput);
        int setTimeLim(string userInput);
        int listJobs(string userInput);
        int bringToForeground(string userInput);
        int makePipeJob(string userInput);
        int fileWriteOperation(string userInput);
        int saveVariable(string userInput);


        //Shell commands:
        string cmdSetPath;
        string cmdSetDataPath;
};

#endif
