
#include "shell.h"
#include <sys/wait.h>
using namespace std;

int main() {
    Shell shell;
    shell.orderLoop();
    return 0;
}

Shell::Shell() {
    initShell();
    updateCurrentPath();
    cmdSetPath = "PATH=";
    cmdSetDataPath = "DATA=";
}

/*
 * Initializes the shell.
 * Checks if the shell is interactive.
 * If not, 
 *  
 */
void Shell::initShell(){
    foregroundTerminal = STDIN_FILENO; 
    interactive = isatty(foregroundTerminal); // Checks if refering to terminal
    
    if(interactive){
        //Waits until the shell becomes active.
        shellPGID = getpgrp();
        while (tcgetpgrp(foregroundTerminal) != shellPGID){
            kill(-shellPGID, SIGTTIN);            
            shellPGID = getpgrp();
        }
        
       //Ignores interactive and job signals:
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);

        //Put our shell in its own process group
        shellPGID = getpid();
        if(setpgid (shellPGID, shellPGID) < 0){
            cout << "ERROR: Could not put the shell in it's own process group";
            exit(1);
        }
        
        //Grabs controll of the terminal. We are now the foreground terminal.
        tcsetpgrp(foregroundTerminal, shellPGID);
        
        /* Save default terminal attributes for shell.  */
        tcgetattr (foregroundTerminal, &shellMode);
        
    }

}

void Shell::updateCurrentPath() {
    char *currentPathPtr = currentPath;
    getcwd(currentPathPtr, 1024);
}

void Shell::orderLoop() {
    string userInput;

    while (true) {
        cout << "SHELL-MOFO - >>> ";
        getline(cin, userInput);
        cout << "\n";
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


