#include <string>


class History{

private:
   string commands[10];
   int lastGet;
   int MAX_HISTORY;

public:
   History();
   string getNext();
   string getPrevious();
   void add(string newCommand);

};
