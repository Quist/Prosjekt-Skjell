
using namespace std;

#include <cstdlib>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>

//Program includes:
#include "history.h"

class Shell{
    
 public:
    void orderLoop();
    Shell();
    
private:
    char currentPath[1024]; 
    
    void updateCurrentPath();
    void handleUserInput(string userInput);
    void readFile(string fileName);
    void startProcess();
    
    
    
    //Shell commands:
    string cmdSetPath;
    string cmdSetDataPath;    
};