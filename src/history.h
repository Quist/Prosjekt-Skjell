#include <string>
#include <map>
#include <sys/stat.h>
#include <string.h>

class History{

private:
   string commands[10];
   int exitStat[10];
   int lastGetCommand;
   int maxHistoryCommand;
   int maxHistoryExitStat;
   map<string, string> variables;
   map<string, string>::iterator it;

public:
   History();

   string getNextCommand();
   string getPreviousCommand();
   void addCommand(string newCommand);

   int getExitStat(int i);
   void addExitStat(int newExitStat);
   void saveVariable(string input);
   char *getVariable(string input);

};
