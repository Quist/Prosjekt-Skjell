
using namespace std;

#include <cstdlib>
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <list>
#include <sys/stat.h>;

//Program includes:
#include "history.h"

class Shell{
    
 public:
    void orderLoop();
    Shell();
    
private:
    char currentPath[1024]; 
    
	void setStartPath();
    void updateCurrentPath(char newPath[]);
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
