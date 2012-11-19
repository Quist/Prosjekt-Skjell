
#include "main.h"

using namespace std;

int main() {
    Main main;
    main.orderLoop();
    return 0;
}

void Main::updateCurrentPath() {
    char *currentPathPtr = currentPath;
    getcwd(currentPathPtr, 1024);
}

void Main::orderLoop() {
    string userInput;
    updateCurrentPath();
    while (true) {
        cout << currentPath << ": ";
        getline(cin, userInput);
        cout << "You typed: " << userInput << "\n";
    }
}