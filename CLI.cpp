// CLI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include "include/ShellHost.h"

using namespace std;

int main()
{
    // read text File
    string userType, hostType, userAnswer, nextPath, line;
    ifstream inFile;
    inFile.open("ShellScreen.txt");
    if (!inFile)
    {
        cout << "Unable to open File";
        exit(1);
    }

    // container holding all paths
    vector<string> pathways;

    getline(inFile, userType);
    getline(inFile, hostType);
    // getting pathways and placing in vector
    while (getline(inFile, line))
    {
        stringstream ss(line);
        while (getline(ss, nextPath, ','))
        {
            // check to remove 'PATH='
            char pathAddress = nextPath.find('=');
            nextPath = nextPath.substr(pathAddress + 1, nextPath.length());
            pathways.push_back(nextPath);
        }
    }
    inFile.close();

    // Initialize shell host object
    // TODO: Parse & pass PATH to shell. For now, give it an empty string vector.
    ShellHost shell(userType, hostType, pathways);

    // Start shell and block until it exits
    thread tshell(shell);
    tshell.join();

    return 0;
    
}
