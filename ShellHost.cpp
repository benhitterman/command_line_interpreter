#include "include/ShellHost.h"

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
                    current_path(path[i]);
                    vector<string> executable;
                    executable.push_back(command);
                    for (const auto &file : executable)
                    {
                        exists(file) ? cout << "Exists\n" : cout << "Doesn't exist\n";
                        // TODO: execute found file
                    }
                }
            }
            // only if no exe is found through all paths and current path
            cout << command << " is not recognized as an internal or external command, operable program or batch file." << endl;
        }
    }
}
