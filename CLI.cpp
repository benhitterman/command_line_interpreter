// CLI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
using namespace std;
//
void shell();
void echo1(string);

int main()
{
   //Call shell thread
   // thread tmain (shell());
   shell();
}
void shell() {

    //read text File
    string userType, hostType, userAnswer;
    ifstream inFile;
    inFile.open("ShellScreen.txt");
    if (!inFile) {
        cout << "Unable to open File";
        exit(1);
    }
    inFile >> userType >> hostType;
    //Shell loop
    while (1) {
        cout << userType << "@" << hostType << "$ ";
        getline(cin, userAnswer);
        //getline(cin, userAnswer, ' ');
        if (userAnswer == "echo") {
            echo1(userAnswer);
        }
        else if (userAnswer == "->") {

        }
        else if (userAnswer == "->>") {

        }
        else if (userAnswer == "&") {

        }
        else if (userAnswer == "exit") {
            exit(1);
        }
        else {
            cout << userAnswer << " is not recognized as an internal or external command, operable program or batch file." << endl;
        }
    }
}
void echo1(string call) {
    cout << "echo choo   o o o o " << endl;
}

