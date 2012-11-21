
#include "shell.h"
#include <sys/wait.h>
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
    } else {
        startProcess();
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

void Shell::startProcess(){
    pid_t pid;
    int status;
    char test[] = "ls";
    
    char* ptr[1024];
    for(int i = 0; i < 1024; i++){
        ptr[i] = &currentPath[i];
    }
        
    char *args[] = {"1", "\0"};
    
    if( (pid = fork()) < 0){
        //fork failed
        cout <<"Fork Failed";
        exit(1);
    }
    else if(pid == 0){   //This is done by the child process
        if( execvpe(test, args, ptr) < 0){
            cout << "Command failed";
            exit(1);
        }
    } else {
        while (wait(&status) != pid)  ;
    }
}

