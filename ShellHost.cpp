#include "include/ShellHost.h"
#include "include/ExecutionUtils.h"

#include <iostream>
#include <filesystem>
#include <sstream>
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

ShellHost::~ShellHost()
{
    for (auto& backgroundThread : backgroundThreads)
    {
        backgroundThread->join();
        delete backgroundThread;
    }
}

void ShellHost::operator()()
{
    while (1)
    {
        string command, fileName, argument;
        cout << user << "@" << host << "$ ";
        getline(cin, command);
        // getline(cin, userAnswer, ' ');

        ParsedInput parsedInput;
        try
        {
            parsedInput = ParseUserInput(command);
        }
        catch (const std::out_of_range& ex)
        {
            cout << "Error: Malformed command." << endl;
            cout << "Please enter a command of the form '[executable] [arguments] [-> file/to/overwrite]/[->> file/to/append] [&]'" << endl;
            continue;
        }
        

        if (command == "echo")
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
                        if (entry.path().stem().string() == parsedInput.getExecutable())
                        {
                            stringstream commandBuilder;
                            commandBuilder << '"' << path[i] << '/' << parsedInput.getExecutable() << '"' << ' ' << parsedInput.getArguments();
                            
                            ExecutionUtils::executeCommandWithConsoleOutput(commandBuilder.str());
                            doneExecution = true;
                            break;
                        }
                    }
                }
            }
            // checks current directory
            for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
            {
                if (entry.path().stem().string() == parsedInput.getExecutable() && !doneExecution)
                {
                    doneExecution = true;
                    ExecutionUtils::executeCommand(parsedInput.getExecutable() + " " + parsedInput.getArguments());
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

ParsedInput ShellHost::ParseUserInput(const std::string& input) const
{
    string fileName, argument;

    size_t firstWhitespace = input.find(" ");
    if (firstWhitespace == string::npos)
    {
        // Only an executable, no args or anything else
        return ParsedInput(input);
    }
    string command1 = input.substr(0, firstWhitespace);

    size_t ampersand = input.rfind("&");
    bool isBackground = 0;
    if (ampersand != string::npos)
    {
        isBackground = 1;
    }

    size_t firstArrow = input.find("->");
    size_t firstDoubleArrow = input.find("->>");

    if (firstDoubleArrow != string::npos)
    {
        argument = input.substr(firstWhitespace + 1, firstDoubleArrow - firstWhitespace - 2);
        if (!isBackground)
        {
            fileName = input.substr(firstDoubleArrow + 4, input.length());
        }
        else
        {
            fileName = input.substr(firstDoubleArrow + 4, ampersand - firstDoubleArrow - 5);
        }
    }
    else if (firstArrow != string::npos)
    {
        argument = input.substr(firstWhitespace + 1, firstArrow - firstWhitespace - 2);
        if (!isBackground)
        {
            fileName = input.substr(firstArrow + 3, input.length());
        }
        else
        {
            fileName = input.substr(firstArrow + 3, ampersand - firstArrow - 4);
        }
    }
    else
    {
        fileName = "";
        if (!isBackground)
        {
            argument = input.substr(firstWhitespace + 1, input.length());
        }
        else
        {
            argument = input.substr(firstWhitespace + 1, ampersand - firstWhitespace - 2);
            cout << "";
        }
    }

    return ParsedInput(command1, argument, fileName, firstDoubleArrow != string::npos, isBackground);
}
