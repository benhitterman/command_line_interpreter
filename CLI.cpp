// CLI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include "include/ShellHost.h"

using namespace std;

int main()
{
    //read text File
    string userType, hostType, userAnswer;
    ifstream inFile;
    inFile.open("ShellScreen.txt");
    if (!inFile) {
        cout << "Unable to open File";
        exit(1);
    }
    inFile >> userType >> hostType;
    
    // Initialize shell host object
    // TODO: Parse & pass PATH to shell. For now, give it an empty string vector.
    ShellHost shell(userType, hostType, vector<string>());

    // Start shell and block until it exits
    thread tshell(shell);
    tshell.join();

    return 0;
}
