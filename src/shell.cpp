
#include "shell.h"
#include <sys/wait.h>
using namespace std;

int main() {
    Shell shell;
    shell.orderLoop();
    return 0;
}

Shell::Shell() {

    setStartPath();
    cmdSetPath = "PATH=";
    cmdSetDataPath = "DATA=";
}

void Shell::setStartPath() {
    char *currentPathPtr = currentPath;
    getcwd(currentPathPtr, 1024);
}

void Shell::updateCurrentPath(char newPath[]) {
	struct stat st;
	if(stat(newPath, &st) == 0 && (((st.st_mode) & S_IFMT) == S_IFDIR)) {
		strncpy(currentPath, newPath, 1024);
	} else {
		cout<<"Directory does not exist.";
	}
}

void Shell::orderLoop() {
    string userInput;

    while (true) {
        cout << "SHELL-MOFO - >>> ";
        getline(cin, userInput);
        handleUserInput(userInput);
    }
}

void Shell::handleUserInput(string userInput) {
    if (userInput.compare(0, cmdSetDataPath.length(), cmdSetDataPath) == 0) {
        cout << "SUCCESS!\n";
    } else {
        const char* cmd = userInput.c_str();
        startProcess(cmd);
    }
}

void Shell::startProcess(const char* command) {
    pid_t pid;
    int status;

    char *const parmList[] = {"", NULL};
    if ((pid = fork()) < 0) {
        //fork failed
        cout << "Fork Failed\n";
        exit(1);
    } else if (pid == 0) { //This is done by the child process
        if (execvp(command, parmList) < 0) {
            cout << "Command failed\n";
            exit(1);
        }
    } else {
        while (wait(&status) != pid);
    }
}

void Shell::exampleStartProcess() {
    pid_t pid;
    int status;

    char *const parmList[] = {"/bin/ls/", "-l", NULL};

    if ((pid = fork()) < 0) {
        //fork failed
        cout << "Fork Failed";
        exit(1);
    } else if (pid == 0) { //This is done by the child process
        if (execvp("lsMOFO", parmList) < 0) {
            cout << "Command failed\n";
            exit(1);
        }
    } else {
        while (wait(&status) != pid);
        cout << "SYSYEM CALL IS FINISHED, motherfucker\n";
    }
}

void Shell::readFile(string fileName) {
    string cmd;
    ifstream inFile;
    inFile.open(fileName.c_str());

    if (inFile.is_open()) {
        while (!inFile.eof()) {
            getline(inFile, cmd);
            handleUserInput(cmd);
        }
    } else {
        cout << "Cant find " + fileName + "\n";
    }
    inFile.close();
}

void Shell::writeToFile(string fileName, list<string> l) {
    string tmp;
    ofstream toFile;
    toFile.open(fileName.c_str(), ios::app);
    while (!l.empty()) {
        tmp = l.front();
        l.pop_front();
        toFile << tmp << endl;
    }
    toFile.close();
}


