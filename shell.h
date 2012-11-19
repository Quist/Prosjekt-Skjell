
using namespace std;

#include <cstdlib>
#include <string>
#include <iostream>


#ifndef SHELL_H
#define	SHELL_H

class Shell{
    //Private declarations(By default):
    char currentPath[1024]; 
    void updateCurrentPath();
    
public:
    void orderLoop();
    
};


#endif	/* MAIN_H */

