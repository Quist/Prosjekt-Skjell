
#include "main.h"

using namespace std;

void getCurrentPath(char *currentPath){
    	getcwd(currentPath, 1024);
}

int main() {
	string userOrder;
        
	char currentPath[1024];
        getCurrentPath(currentPath);
	while(true){
		cout << currentPath << ": ";
		getline(cin, userOrder);
		cout << "You typed: " << userOrder << "\n";
	}
	return 0;
}



