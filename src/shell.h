
using namespace std;

#include <cstdlib>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <list>
#include <termios.h>

//Program includes:
#include "history.h"

class Shell{
    
 public:
    void orderLoop();
    Shell();
    
private:
    
    pid_t shellPGID;    
    struct termios shellMode;
    int foregroundTerminal;
    int interactive;
    
    char currentPath[1024];     
    
    void initShell();
    void updateCurrentPath();
    void handleUserInput(string userInput);
    void readFile(string fileName);
    void exampleStartProcess();
    void startProcess(const char* command);

    void startProcess();
    void writeToFile(string fileName, list<string> l);
    
    
    //Shell commands:
    string cmdSetPath;
    string cmdSetDataPath;    
};
