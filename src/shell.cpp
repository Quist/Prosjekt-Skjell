
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
        cout << "SHELL-MOFO - >>> ";
        getline(cin, userInput);
        handleUserInput(userInput);
    }
}

void Shell::handleUserInput(string userInput){
    if(userInput.compare(0, cmdSetDataPath.length(), cmdSetDataPath) == 0){
        cout << "SUCCESS!\n";
    } else {
        const char* cmd = userInput.c_str();
        startProcess(cmd);
    }
}



void Shell::startProcess(const char* command){
    pid_t pid;
    int status;  
    
    char  *const parmList[] = {"", NULL};
    if( (pid = fork()) < 0){
        //fork failed
        cout <<"Fork Failed\n";
        exit(1);
    }
    else if(pid == 0){   //This is done by the child process
        if( execvp(command, parmList) < 0){
            cout << "Command failed\n";
            exit(1);
        }
    } else {
        while (wait(&status) != pid)  ;
    }
}



void Shell::exampleStartProcess(){
    pid_t pid;
    int status;  
    
    char  *const parmList[] = {"/bin/ls/", "-l", NULL};
    
    if( (pid = fork()) < 0){
        //fork failed
        cout <<"Fork Failed";
        exit(1);
    }
    else if(pid == 0){   //This is done by the child process
        if( execvp("lsMOFO", parmList) < 0){
            cout << "Command failed\n";
            exit(1);
        }
    } else {
        while (wait(&status) != pid)  ;
        cout <<"SYSYEM CALL IS FINISHED, motherfucker\n";
    }
}

void Shell::readFile(string fileName) {
	string cmd;
	ifstream inFile;
	//inFile.open(fileName);
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