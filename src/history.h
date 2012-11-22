#include <string>


class History{

private:
   string commands[10];
   string exitStat[10];
   int lastGetCommand;
   int maxHistoryCommand;
   int lastGetExitStat;
   int maxHistoryExitStat;

public:
   History();

   string getNextCommand();
   string getPreviousCommand();
   void addCommand(string newCommand);

   string getNextExitStat();
   string getPreviousExitStat();
   void addExitStat(string newExitStat);

};
