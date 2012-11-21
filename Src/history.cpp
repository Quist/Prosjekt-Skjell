using namespace std;

#include "history.h"

History::History() {
   lastGet = 0;
   MAX_HISTORY=10;
}

void History::add(string newCommand) {

   for(int i = 0; i < MAX_HISTORY; i++) {
      commands[i+1] = commands[i];  
   }

   commands[0] = newCommand;

   lastGet = 0;
}

string History:: getNext() {
   if(lastGet == MAX_HISTORY) {
      return commands[MAX_HISTORY];
   }
   else {
      return commands[lastGet++];
   }
}

string History:: getPrevious() {
   if(lastGet == 0) {
      return commands[0];
   }
   else {
      return commands[--lastGet];
   }
}
