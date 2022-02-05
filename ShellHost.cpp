#include "include/ShellHost.h"

#include <iostream>

using namespace std;

ShellHost::ShellHost(std::string user, std::string host, std::vector<std::string> path)
    : user(user)
    , host(host)
    , path(path)
{}

void ShellHost::operator()()
{
    while (1)
    {
        string command;
        cout << user << "@" << host << "$ ";
        getline(cin, command);
        //getline(cin, userAnswer, ' ');
        if (command == "echo") {
        }
        else if (command == "->") {

        }
        else if (command == "->>") {

        }
        else if (command == "&") {

        }
        else if (command == "exit") {
            return;
        }
        else {
            cout << command << " is not recognized as an internal or external command, operable program or batch file." << endl;
        }
    }
}
