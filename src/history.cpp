using namespace std;

#include "history.h"

History::History() {
   lastGetCommand = 0;
   maxHistoryCommand = 0;
   
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

   if(maxHistoryExitStat < 10)
      maxHistoryExitStat++;
}

string History::getExitStat(int i) {
   if(i <= maxHistoryExitStat) {
      if(i > 0) {
         return exitStat[i];
      }
   }
}

void History::saveVariable(string input) {
	size_t found;
	string key;
	string path;

	found = input.find("=");
	key = input.substr(0, found);
	path = input.substr(found + 1, input.length());

	//if(dirChecker(path)) {
	//	variables.insert(pair<string,string>(key,path));
	//}
}

char* History::getVariable(string input) {
	string tmp;

	it = variables.find(input);
	tmp = it->second;
	char *output = (char*)tmp.c_str();
	return output;
}
