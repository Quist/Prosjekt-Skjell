
using namespace std;

#include <cstdlib>
#include <string>
#include <iostream>
#include <unistd.h>

class Shell{
private:
    char currentPath[1024]; 
    void updateCurrentPath();
    
public:
    void orderLoop();
    
};
