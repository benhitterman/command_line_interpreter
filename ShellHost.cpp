#include "include/ShellHost.h"
#include "include/ExecutionUtils.h"

#include <iostream>
#include <filesystem>
#include <sys/stat.h>
using namespace std;

bool isPathExist(const string &s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

ShellHost::ShellHost(std::string user, std::string host, std::vector<std::string> path)
    : user(user), host(host), path(path)
{
}

void ShellHost::operator()()
{
FoundExec:
    while (1)
    {
        string command, fileName, argument;
        cout << user << "@" << host << "$ ";
        getline(cin, command);
        // getline(cin, userAnswer, ' ');

        char firstWhitespace = command.find(" ");
        string command1 = command.substr(0, firstWhitespace);

        char ampersand = command.rfind("&");
        bool isBackground = 0;
        if (ampersand != string::npos)
        {
            isBackground = 1;
        }

        char firstArrow = command.find("->");
        char firstDoubleArrow = command.find("->>");

        if (firstDoubleArrow != string::npos)
        {
            argument = command.substr(firstWhitespace + 1, firstDoubleArrow - firstWhitespace - 2);
            if (!isBackground)
            {
                fileName = command.substr(firstDoubleArrow + 4, command.length());
            }
            else
            {
                fileName = command.substr(firstDoubleArrow + 4, ampersand - firstDoubleArrow - 5);
            }
        }
        else if (firstArrow != string::npos)
        {
            argument = command.substr(firstWhitespace + 1, firstArrow - firstWhitespace - 2);
            if (!isBackground)
            {
                fileName = command.substr(firstArrow + 3, command.length());
            }
            else
            {
                fileName = command.substr(firstArrow + 3, ampersand - firstArrow - 4);
            }
        }
        else
        {
            fileName = "";
            if (!isBackground)
            {
                argument = command.substr(firstWhitespace + 1, command.length());
            }
            else
            {
                argument = command.substr(firstWhitespace + 1, ampersand - firstWhitespace - 2);
                cout << "";
            }
        }

        if (command == "echo")
        {
        }
        else if (command == "->")
        {
        }
        else if (command == "->>")
        {
        }
        else if (command == "&")
        {
        }
        else if (command == "exit")
        {
            return;
        }
        else
        {
            bool doneExecution = false;
            for (int i = 0; i < path.size() && !doneExecution; i++)
            {
                if (isPathExist(path[i]))
                {
                    // checks if file exists in path
                    for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(path[i]))
                    {
                        if (entry.path().stem().string() == command)
                        {
                            ExecutionUtils::executeCommand(command);
                            doneExecution = true;
                            break;
                        }
                    }
                }
            }
            // checks current directory
            for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
            {
                if (entry.path().stem().string() == command && !doneExecution)
                {
                    doneExecution = true;
                    ExecutionUtils::executeCommand(command);
                    break;
                }
            }
            if (!doneExecution)
            {
                // only if no exe is found through all paths and current path
                cout << command << " is not recognized as an internal or external command, operable program or batch file." << endl;
            }
        }
    }
}
