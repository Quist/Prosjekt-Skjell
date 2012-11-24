
using namespace std;

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
#include <time.h>

//Program includes:
#include "history.h"
#include "process.cpp"

class Shell {
public:
    void orderLoop();
    Shell();

private:

    pid_t shellPGID;
    struct termios shellMode;
    int foregroundTerminal;
    int interactive;

    char currentPath[1024];

    void test(string cmd);
    
    void initShell();
    void setStartPath();
    void updateCurrentPath(char newPath[]);
    void handleUserInput(string userInput);
    void readFile(string fileName);
    
    void exampleStartProcess();
    void startProcess(const char* command);
    void startProcess();
    void launchProcess(Process *p, pid_t pgid, int infile, int outfile,
        int errfile, int foreground);
    
    void writeToFile(string fileName, list<string> l);


    //Shell commands:
    string cmdSetPath;
    string cmdSetDataPath;
};
