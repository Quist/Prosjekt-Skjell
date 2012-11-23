#include <string>


class History{

private:
   string commands[10];
   string exitStat[10];
   int lastGetCommand;
   int maxHistoryCommand;
   int maxHistoryExitStat;

public:
   History();

   string getNextCommand();
   string getPreviousCommand();
   void addCommand(string newCommand);

   string getExitStat(int i);
   void addExitStat(string newExitStat);

};
