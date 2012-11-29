
#include "shell.h"

#include <stdio.h>
using namespace std;

int main() {
    Shell shell;
    shell.orderLoop();
    return 0;
}

Shell::Shell() {
    History his;
    initShell();
    setStartPath();
    cmdSetPath = "PATH=";
    cmdSetDataPath = "DATA=";
    jobCount = 0;
    firstJob = NULL;
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
    char *currentDataPtr = currentData;
    string cmd;
    size_t found, found2;
    ifstream inFile;
    inFile.open(".sh142");

    if(inFile.is_open()) {
        while(!inFile.eof()) {
            getline(inFile, cmd);

            found = cmd.find("DATA=");
            if(found != string::npos) {
                strcpy(currentData, cmd.c_str());
            }

            found = cmd.find("PATH=");
            if(found != string::npos) {
                strcpy(currentPath, cmd.c_str());
            }
        }
    } else {
        getcwd(currentPathPtr, 1024);
        getcwd(currentDataPtr, 1024);
    }
    inFile.close();
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
        cout << "sh142: ";
        getline(cin, userInput);
        if((userInput.compare(0, 4, "exit") == 0)) {
            exit(0);
        }
        checkCommand(userInput);
        //handleUserInput(userInput);
    }
}

void Shell::checkCommand(string userInput){
    size_t posAmp;
    size_t posOr;
    size_t forLoop;
    size_t variable;
    size_t redirLess;
    size_t redirBigger;
    size_t redirLessTwo;

    string tmp = "";

    posAmp = userInput.find("&&");
    posOr = userInput.find("||");
    forLoop = userInput.find("For(");
    variable = userInput.find("$");
    redirLess = userInput.find("<");
    redirBigger = userInput.find(">");
    redirLessTwo = userInput.find("2>");


    if(userInput.length() == 0){
        //No command given
    }else if(userInput.compare(userInput.length()-1, 1, "&") == 0){
        //check this commmand but this time with only the commands before
        //the '&'
        //TODO make sure this command runs in background!
        prepareJob(userInput.substr(0, userInput.length()-1), 0);
        cout << "This process will be run in background" << endl;
    }else if(posAmp != string::npos){

        string cmd = userInput.substr(0, posAmp); 
        commands.push_front(cmd);
        cmd = userInput.substr(posAmp);
        commands.push_front(cmd);

    }else if(posOr != string::npos){

    }else if(forLoop != string::npos){
        int incrementing = 0;
        int start;
        int goesTo;
        int lessThan;
        string lengthTmp;
        string startVal;
        string command;

        size_t pos = userInput.find("(");

        string tmp = userInput.substr(pos);

        pos = tmp.find(")");
        command = tmp.substr(pos+2,tmp.length());
        tmp = tmp.substr(0, (int) pos);

        if(strncmp(tmp.substr(tmp.length()-2, tmp.length()).c_str(), "++", 2) == 0){
            incrementing = 1;
        }else if(strncmp(tmp.substr(tmp.length()-2, tmp.length()).c_str(), "--", 2) == 0){
            incrementing = 0;
        }

        pos = tmp.find(";");
        lengthTmp = tmp.substr(pos);

        tmp = tmp.substr(1, pos-1);
        pos = tmp.find("=");
        startVal = tmp.substr(pos+1);
        start = atoi(startVal.c_str());

        pos = lengthTmp.find("<");
        if(pos == string::npos){
            lessThan = 0;
            pos = lengthTmp.find(">");
        }else{
            lessThan = 1;
        }

        if(pos != string::npos){
            lengthTmp = lengthTmp.substr(pos);
            pos = lengthTmp.find(";");
            lengthTmp = lengthTmp.substr(1,pos-1);
            goesTo = atoi(lengthTmp.c_str());
        }else{
            return;
        }
        cout << "Command: " << command << "\nStart: " << start << "\nGoesTo: " << goesTo << "\nIncr: " << incrementing << "\nLess?: " << lessThan << endl;
        execForLoop(command, start, goesTo, incrementing, lessThan);



    }else{
        if(variable != string::npos && userInput.substr((int)variable+1,(int)variable+1) != "?"){
            string vari; 
            int count = 0;
            int scnt = 0;
            tmp = userInput.substr(variable);
            while(vari.empty()){
                count++;
                vari = his.getVariable(tmp.substr(1 ,count));
            }
            userInput = userInput.replace((int)variable, count+1, vari);

            cout << userInput << endl;

        }

        handleUserInput(userInput);

    }
}

void Shell::execForLoop(string command, int start, int end, int increment, int lessThan){
    if(lessThan){
        if(increment){
            for(int i = start; i < end; i++){
                checkCommand(command);
            }
        }else{
            for(int i = start; i < end; i--){
                checkCommand(command);
            }
        }
    }else{
        if(increment){
            for(int i = start; i > end; i++){
                checkCommand(command);
            }
        }else{
            for(int i = start; i > end; i--){
                checkCommand(command);
            }
        }
    }
}


int Shell::setPathOrData(string userInput){
    if(userInput.length() > 5){

        if(userInput.substr(0,5) == cmdSetPath){
            //set a persistent executable path

            //TODO setEnvironment(userInput);
            return 1;

        }else if(userInput.substr(0,5) == cmdSetDataPath){
            //set a persistet data file path
            //TODO setEnvironment(userInput);
            return 1;

        }
    }
    return 0;
}

int Shell::findCommandNum(string userInput){
    //copying string to charArray
    char *input = new char[userInput.size() + 1];
    input[userInput.size()] = 0;
    memcpy(input, userInput.c_str(), userInput.size());

    if(strncmp (input, "$?x", 2) == 0){
        //check exit status for the command# after $?
        int cmd = input[2];
        if(cmd > 47 && cmd < 58){
            cmd = cmd - 48;
            cout << "use this method: his.getExitStat(cmd);" << endl;
            cout << "u gave wanted exit status for command : " << cmd << endl;
            return 1;

        }else{
            cout << "Could not find command#: " << input[3] << endl; 
            return 1;

        }

    }
    return 0;
}

int Shell::setCPUorMEM(string userInput){
    string tmp;
    size_t pos;
    if(userInput.length() > 7){

        if(userInput.substr(0, 7) == "CPUMAX="){
            //sets maximum CPU utilization
            pos = userInput.find("=");
            tmp = userInput.substr(pos);
            tmp = tmp.substr(1, tmp.length());

            pos = tmp.find(":");
            int percentage = atoi(tmp.substr(0, (int) pos).c_str());
            int seconds = atoi(tmp.substr((int) pos + 1, tmp.length()).c_str());

            cout << "use this method: restrictCPU(seconds, percentage)" << endl;
            cout << "in ur case:" << endl;
            cout << "seconds = " << seconds << endl;
            cout << "percentage = " << percentage << endl;
            return 1;

        }else if(userInput.substr(0,7) == "MEMMAX="){
            //Assuming you can only set megabytes restriction
            pos = userInput.find("=");
            tmp = userInput.substr(pos);
            tmp = tmp.substr(1, tmp.length());

            pos = tmp.find(":");
            int megaBytes = atoi ( tmp.substr(0, (int) pos -1).c_str()); 
            int seconds = atoi ( tmp.substr((int) pos + 1, tmp.length()).c_str());

            cout << "use this method: restrictMEM(megaBytes, seconds)" << endl;
            cout << "in ur case:" << endl;
            cout << "megaBytes = " << megaBytes << endl;
            cout << "seconds = " << seconds << endl;
            return 1;
        }

    }
    return 0;

}

int Shell::setTimeLim(string userInput){
    string tmp;
    size_t pos;
    if(userInput.length() > 8 && userInput.substr(0, 8) == "TIMEMAX="){
        //restricts running time of program
        pos = userInput.find("=");
        tmp = userInput.substr(pos);
        tmp = tmp.substr(1, tmp.length());

        int seconds = atoi(tmp.c_str());

        cout << "use this method to restrict running time for a prog: JobControl::setTimeLimit(seconds)" << endl;
        cout << "in ur case:" << endl;
        cout << "seconds = " << seconds << endl;
        return 1;

    }
    return 0;
}

int Shell::listJobs(string userInput){
    if(userInput.compare(0, userInput.length(), "listjobs") == 0){
        showJobs();
        return 1;
    }
    return 0;
}

int Shell::bringToForeground(string userInput){
    if(userInput.length() > 3 && userInput.substr(0,2) == "fg"){
        string tmp = userInput.substr(3, userInput.length());
        int jobNum = atoi (tmp.c_str());
        bringJobToForeground(jobNum);
        return 1;
        //TODO new command: fg tall, vil bare ha int'en i method: bringJobToForeground(int i);
    }
    return 0;
}

int Shell::makePipeJob(string userInput){
    string tmp;
    string tmp2;
    size_t pos;

    if(userInput.find("|") != string::npos){
        list<string> cmds;
        pos = userInput.find("|");
        tmp = userInput.substr(0, pos);
        tmp2 = userInput.substr(pos+2);

        cmds.push_front(tmp);

        while(pos != string::npos){
            pos = tmp2.find("|");
            tmp = tmp2.substr(0,pos);
            tmp2 = tmp2.substr(pos+2);
            cmds.push_back(tmp);
        }

        cout << "preparePipeJob(cmds);" << endl;
        return 1;
    }
    return 0;
}

int Shell::fileWriteOperation(string userInput){
    string tmp;
    string tmp2;
    size_t pos;

    if(userInput.find("<") != string::npos || userInput.find(">") != string::npos || userInput.find("2>") != string::npos){
        pos = userInput.find("<");
        int fileType = 0;
        if(pos == string::npos){
            pos = userInput.find(">");
            fileType = 1;
            if(pos == string::npos){
                pos = userInput.find("2>");
                fileType = 2;
            }
        }

        tmp = userInput.substr(0, pos);
        tmp2 = userInput.substr(pos+2);

        //TODO YOLO
        return 1;
    }
    return 0;
}

int Shell::saveVariable(string userInput){

    if(userInput.find("=") != string::npos){
        his.saveVariable(userInput); 
        return 1;
    }
    return 0;
}

void Shell::handleUserInput(string userInput) {

    if(setPathOrData(userInput)){
        cout << "setter path eller data" << endl;
    }else if(findCommandNum(userInput)){
        cout << "finner command#" << endl;
    }else if(setCPUorMEM(userInput)){
        cout << "setter CPU eller MEM" << endl;
    }else if(setTimeLim(userInput)){
        cout << "setter TimeLimit" << endl;
    }else if(listJobs(userInput)){
        cout << "lister jobber" << endl;
    }else if(bringToForeground(userInput)){
        cout << "sender til foreground" << endl;
    }else if(makePipeJob(userInput)){
        cout << "lager pipe job" << endl;
    }else if(fileWriteOperation(userInput)){
        cout << "skrive operation" << endl;
    }else if(saveVariable(userInput)){
        cout << "saver variable" << endl;
    }else{
        prepareJob(userInput, 1);
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

void Shell::prepareJob(string cmd, int foreground) {
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

    Job *j = new Job(p, cmd);

    launchJob(j, foreground);
}

void Shell::launchJob(Job *j, int foreground) {

    Process *p = j->firstProcess;
    addJob(j);
    pid_t pid;
    int outfile;
    int infile = j->stdin;
    int mypipe[2];

    for(p = j->firstProcess; p; p = p-> next) {

        /*setting up pipes*/
        if (p->next) {
            cout << "CAME HERE!";

            if (pipe(mypipe) < 0) {
                perror ("pipe");
                exit(1);
            }
            outfile = mypipe[1];
        }
        else {
            outfile = j->stdout;
        }

        /* forking the child process */
        pid = fork();

        //Child:
        if(pid == 0){      
            launchProcess(p, j->pgid, infile, outfile, j->stderr, foreground);
        } 
        else if (pid < 0) {
            perror("fork");
            exit(1);
        } 
        else { //only for the parent process
            p->pid = pid;
            if(interactive){

                //Sets the jobs process group id to the first process pid.
                if(!j->pgid){
                    j->pgid = pid;
                    setpgid (pid, j->pgid);
                }            
            }        
        }

        //closing the file if piping is done
        if (infile != j-> stdin) {
            close(infile);
        }
        if (outfile != j->stdout) {
            close(outfile);
        }

        infile = mypipe[0];

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
        his.addExitStat(status);
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

    //Executes the new process and only returns if error
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

bool Shell::dirChecker(char dir[]) {
    struct stat st;
    if(stat(dir, &st) == 0 && (((st.st_mode) & S_IFMT) == S_IFDIR)) {
        return true;
    }
    return false;
}

void Shell::showJobs() {
    Job *j;
    for (j = firstJob; j; j = j->nextJob) {
        cout << j->pgid << " \t" << j->name << "\n";
    }
}

void Shell::addJob(Job *j) {
    if (!firstJob) {
        firstJob = j;
    }
    else {
        Job *jobTemp = firstJob;

        while(jobTemp->nextJob) {
            jobTemp = jobTemp->nextJob;
        }
        jobTemp->nextJob = j;
    }
}

void Shell::bringJobToForeground(int pgid){
    Job *j = findJob(pgid);
    if(j){
        putJobInForeground(j, 1);
    }
}

Job* Shell::findJob(int pgid){
    Job *j;
    for(j = firstJob; j; j = j->nextJob){
        if(j->pgid == pgid ){
            return j;
        }
    }
    return NULL;
}

void Shell::killJob(int pgid) {
    Job *j = findJob(pgid);
    if (j == NULL) {
        return;
    }
    kill(j->pgid, SIGKILL);
}

void Shell::removeJob(int pgid) {
    Job *j = firstJob;

    if(!firstJob){
        return;
    }

    if(pgid == firstJob->pgid){
        firstJob = firstJob->nextJob;
    }
    for (j = firstJob; j; j = j->nextJob) {
        if (j->nextJob) {
            if (j->nextJob->pgid == pgid) {
                j->nextJob = j->nextJob->nextJob;
            }
        }

    }
}
