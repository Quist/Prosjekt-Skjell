
#include "shell.h"


using namespace std;

int main() {
    Shell shell;
    shell.orderLoop();
    return 0;
}

Shell::Shell() {
    initShell();
    setStartPath();
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
        if((userInput.compare(0, 4, "exit") == 0)) {
            exit(0);
        }
        handleUserInput(userInput);
    }
}

void Shell::handleUserInput(string userInput) {
    if (userInput.compare(0, cmdSetDataPath.length(), cmdSetDataPath) == 0) {
        cout << "SUCCESS!\n";
    } else {
        const char* cmd = userInput.c_str();
       // startProcess(cmd);
        testJob(userInput);
    }  
}

void Shell::test(string cmd) {

    char *cString;
    char *tokens[10];
    
    cString = new char [cmd.size() + 1];
    strcpy(cString, cmd.c_str());
    char *token = strtok(cString, " ");

    int i = 0;
    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;
    

    Process *p = new Process(&*tokens);
    launchProcess(p, 0, 0,0,0,1 );
}

void Shell::testJob(string cmd) {
    char *cString;
    char *tokens[10];
    
    cString = new char [cmd.size() + 1];
    strcpy(cString, cmd.c_str());
    char *token = strtok(cString, " ");

    int i = 0;
    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;
    

    Process *p = new Process(&*tokens);

    Job *j = new Job(p);

    launchJob(j, 0);
}
    

void Shell::launchJob(Job *j, int foreground){
    Process *p = j->firstProcess;
    pid_t pid;
    int outfile;
    int infile = j->stdin;
    
    pid = fork();
    
     //Child:
    if(pid == 0){      
        launchProcess(p, j->pgid,0,0,0, foreground);
    } else if (pid < 0) {
        perror("fork");
        exit(1);
    } else { //parent:
        p->pid = pid;
        if(interactive){
            
            //Sets the jobs process group id to the first process pid.
            if(!j->pgid){
                j->pgid = pid;
                setpgid (pid, j->pgid);
            }            
        }        
    }

    if(!interactive){
       waitForJob(j); 
    }
    else if(foreground){
        putJobInForeground(j, 0);
    }else{
        putJobInBackground(j,0);
    }
    
}

void Shell::putJobInForeground(Job *j, int cont) {
    //Gives the terminal to the job:
    tcsetpgrp(foregroundTerminal, j->pgid);

    //Sends continue signal to the job:
    if (cont) {
        tcsetattr(foregroundTerminal,TCSADRAIN, &j->tmodes);
        if (kill(-j->pgid, SIGCONT) < 0)
            perror("kill (SIGCONT)");
    }
    
    waitForJob(j);

    //Put the shell back in the foreground.
    tcsetpgrp(foregroundTerminal, shellPGID);

    // Restore the shell's terminal modes.
    tcgetattr(foregroundTerminal, &j->tmodes);
    tcsetattr(foregroundTerminal, TCSADRAIN, &shellMode);

}

void Shell::putJobInBackground(Job *j, int cont) {
    // Send the job a continue signal:
    if (cont) {
        if (kill(-j->pgid, SIGCONT) < 0) {
            perror("kill (SIGCONT)");
        }
    }
}

int Shell::markProcessStatus(pid_t pid, int status){
    Job *j;
    Process *p;
    
    if(pid > 0){
         for(j = firstJob; j; j = j->nextJob){
             for(p = j->firstProcess; p; p= p->nextProcess){
                 /*Maybe change so only the process with the right pid is called.
                  * The check is now done in the process method.
                 */
                 return p->markProcessStatus(pid, status);
             }
         }
    } else if(pid == 0 /*||errno == ECHILD*/){
        //No processes to update
        return -1;
    } else {
        //Error handler:
        perror("waitpid");
        return -1;
    }
}

void Shell::waitForJob(Job *j) {
    int status;
    pid_t pid;

    do {
        pid = waitpid(WAIT_ANY, &status, WUNTRACED);
    } while (!markProcessStatus(pid, status)
            && !jobIsStopped(j)
            && !jobIsCompleted(j));
}

//Return true if all processes in the job have stopped or completed. 
int Shell::jobIsStopped(Job *j) {
    Process *p;
    for (p = j->firstProcess; p; p = p->nextProcess)
        if (!p->completed && !p->stopped)
            return 0;
    return 1;
}

//Return true if all processes in the job have completed.
int Shell::jobIsCompleted(Job *j) {
    Process *p;

    for (p = j->firstProcess; p; p = p->nextProcess)
        if (!p->completed)
            return 0;
    return 1;
}


void Shell::launchProcess(Process *p, pid_t pgid, int infile, int outfile,
        int errfile, int foreground){
    
    pid_t pid;
    
    if(interactive){
        pid = getpid();
        if(pgid == 0){
            pgid = pid;
        }
        setpgid(pid, pgid);
        
        if(foreground){
            tcsetpgrp(foregroundTerminal, pgid);
        }
        
        /* Since the process forked of the terminal and the terminal ignores
         * signals -> We have to set signal handling back to default for the 
         * process         
         */
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);
    }
    
    
    /* Sets file descriptor. Pointer to the details of open files in kernel.
     * Sets the standard input/output channel 
     */
    if (infile != STDIN_FILENO) {
        dup2(infile, STDIN_FILENO);
        close(infile);
    }
    if (outfile != STDOUT_FILENO) {
        dup2(outfile, STDOUT_FILENO);
        close(outfile);
    }
    if (errfile != STDERR_FILENO) {
        dup2(errfile, STDERR_FILENO);
        close(errfile);
    }
    
    //Executes the new process and exits after the execution.
    execvp(p->args[0], p->args);
    perror("execvp");
    exit(1);

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


