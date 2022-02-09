#include "include/ShellHost.h"
#include "include/ExecutionUtils.h"

#include <iostream>
#include <filesystem>
#include <sys/stat.h>
using namespace std;
using filesystem::current_path;
using filesystem::exists;
using filesystem::is_directory;
using filesystem::is_regular_file;
namespace fs = filesystem;

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
        string command;
        cout << user << "@" << host << "$ ";
        getline(cin, command);
        // getline(cin, userAnswer, ' ');
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
            for (int i = 0; i < path.size(); i++)
            {
                if (isPathExist(path[i]))
                {
                    // checks if file exists in path
                    vector<string> executable;
                    // checking if user entered exe again
                    if (command.substr(command.length() - 4, command.length()) != ".exe")
                    {
                        command = command.append(".exe");
                    }
                    // creating full directory
                    string fullExec = "";
                    fullExec.append(path[i]);
                    fullExec.append(command);

                    // adding .exe to end of command
                    executable.push_back(command);
                    for (const auto &file : executable)
                    {
                        if (exists(fullExec))
                        {
                            ExecutionUtils::executeCommand(fullExec);
                            // marks execution found and leaves else statement
                            goto FoundExec;
                        }
                    }
                }
            }
            // checks current directory
            if (exists(command))
            {
                ExecutionUtils::executeCommand(command);
            }
            else
                // only if no exe is found through all paths and current path
                cout << command << " is not recognized as an internal or external command, operable program or batch file." << endl;
        }
    }
}
