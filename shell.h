
using namespace std;

#include <cstdlib>
#include <string>
#include <iostream>


#ifndef MAIN_H
#define	MAIN_H

class Shell{
    //Private declarations(By default):
    char currentPath[1024]; 
    void updateCurrentPath();
    
public:
    void orderLoop();
    
};


#endif	/* MAIN_H */

