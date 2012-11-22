using namespace std;

#include "history.h"

History::History() {
   lastGetCommand = 0;
   maxHistoryCommand = 0;
   
   lastGetExitStat = 0;
   maxHistoryExitStat = 0;
}

void History::addCommand(string newCommand) {

   for(int i = 0; i < maxHistoryCommand; i++) {
      commands[i+1] = commands[i];  
   }

   commands[0] = newCommand;

   lastGetCommand = 0;
   if(maxHistoryCommand < 10)
      maxHistoryCommand++;
}

string History:: getNextCommand() {
   if(lastGetCommand == maxHistoryCommand) {
      return commands[maxHistoryCommand];
   }
   else {
      return commands[lastGetCommand++];
   }
}

string History:: getPreviousCommand() {
   if(lastGetCommand == 0) {
      return commands[0];
   }
   else {
      return commands[--lastGetCommand];
   }
}

void History::addExitStat(string newExitStat) {

   for(int i = 0; i < maxHistoryExitStat; i++) {
      exitStat[i+1] = exitStat[i];  
   }

   exitStat[0] = newExitStat;

   lastGetExitStat = 0;
   if(maxHistoryExitStat < 10)
      maxHistoryExitStat++;
}

string History:: getNextExitStat() {
   if(lastGetExitStat == maxHistoryExitStat) {
      return exitStat[maxHistoryExitStat];
   }
   else {
      return exitStat[lastGetExitStat++];
   }
}

string History:: getPreviousExitStat() {
   if(lastGetExitStat == 0) {
      return exitStat[0];
   }
   else {
      return exitStat[--lastGetExitStat];
   }
}
