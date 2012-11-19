/* 
 * File:   main.cpp
 * Author: joakim
 *
 * Created on November 18, 2012, 6:01 PM
 */

#include "main.h"

using namespace std;

int main() {
	string userOrder;
	char *currentPath = new char[1024];
	char *pathPtr = getcwd(currentPath,1024);
	while(true){
		cout << *pathPtr << ": ";
		getline(cin, userOrder);
		cout << "You typed: " << userOrder << "\n";
	}
	return 0;
}

