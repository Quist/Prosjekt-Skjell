
#include "shell.h"

using namespace std;

int main() {
    Shell shell;
    shell.orderLoop();
    return 0;
}

Shell::Shell(){
    cmdSetPath = "PATH=";
    cmdSetDataPath = "DATA=";
}

void Shell::updateCurrentPath() {
    char *currentPathPtr = currentPath;
    getcwd(currentPathPtr, 1024);
}

void Shell::orderLoop() {
    string userInput;
    updateCurrentPath();
    while (true) {
        cout << currentPath << ": ";
        getline(cin, userInput);
        cout << "You typed: " << userInput << "\n";
        handleUserInput(userInput);
    }
}

void Shell::handleUserInput(string userInput){
    if(userInput.compare(0, cmdSetDataPath.length(), cmdSetDataPath) == 0){
        cout << "SUCCESS!\n";
    }   
}

void Shell::readFile(string fileName) {
	string cmd;
	ifstream inFile;
	inFile.open(fileName);

	if(inFile.is_open()) {
		while(!inFile.eof())
			{
				getline(inFile, cmd);
				handleUserInput(cmd);
			}
	} else
	{
		cout<<"Cant find " + fileName+"\n";
	}
	inFile.close();
}

void startProcess(){
    execvp();
}
