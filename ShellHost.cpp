#include "include/ShellHost.h"
#include "include/ExecutionUtils.h"

#include <iostream>
#include <filesystem>
#include <sstream>
#include <sys/stat.h>
using namespace std;

ShellHost::ShellHost(std::string user, std::string host, std::vector<std::string> path)
    : user(user), host(host), path(path)
{
}

ShellHost::~ShellHost()
{
    // Join & delete all background threads started during the lifetime of the CLI
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
        // Get user input from the command line
        string command, fileName, argument;
        cout << user << "@" << host << "$ ";
        getline(cin, command);
        
        if (command.empty()) continue;

        // Try to break the input down into executable, arguments, output file, background
        ParsedInput parsedInput;
        try
        {
            parsedInput = parseUserInput(command);
        }
        catch (const std::out_of_range& ex)
        {
            cout << "Error: Malformed command." << endl;
            cout << "Please enter a command of the form '[executable] [arguments] [-> file/to/overwrite]/[->> file/to/append] [&]'" << endl;
            continue;
        }
        
        // Handle internal commands "echo" and "exit", otherwise try internal and external
        if (parsedInput.getExecutable() == "echo")
        {
            if (parsedInput.shouldRunInBackground())
            {
                // Create a new thread on the heap and add it to the list of background threads to clean up on exit.
                backgroundThreads.push_back(
                    new thread(
                        ExecutionUtils::echo, 
                        parsedInput.getArguments(), 
                        parsedInput.getOutputFile(),
                        parsedInput.shouldOverwriteOutputFile()));
            }
            else
            {
                // Create a new thread and block until it exits.
                thread execThread(
                    ExecutionUtils::echo, 
                    parsedInput.getArguments(), 
                    parsedInput.getOutputFile(), 
                    parsedInput.shouldOverwriteOutputFile());
                execThread.join();
            }
        }
        else if (parsedInput.getExecutable() == "exit")
        {
            return;
        }
        else
        {
            // Check the path and current directory
            bool success = executeCommand(parsedInput);
            if (!success)
            {
                // only if no exe is found through all paths and current path
                cout << command << " is not recognized as an internal or external command, operable program or batch file." << endl;
            }
        }
    }
}

ParsedInput ShellHost::parseUserInput(const std::string& input) const
{
    string fileName, argument;

    // Everything up to the first whitespace is the executable
    size_t firstWhitespace = input.find(" ");
    if (firstWhitespace == string::npos)
    {
        // Only an executable, no args or anything else
        return ParsedInput(input);
    }
    string command1 = input.substr(0, firstWhitespace);

    // An ampersand as the last non-whitespace char indicates that the command should be run in the background
    size_t lastNonWhitespace = input.find_last_not_of(" \n");
    bool isBackground = (input[lastNonWhitespace] == '&');

    // Everything after the executable name and before the arrow are the arguments
    // Everything after the arrow and before the ampersand/end of string is the filename
    // If there is no arrow, everything after the command until the ampersand/end of string are the arguments
    size_t firstArrow = input.find("->");
    size_t firstDoubleArrow = input.find("->>");
    bool shouldOverwrite = (firstArrow != string::npos) && (firstDoubleArrow == string::npos);
    if (firstDoubleArrow != string::npos)
    {
        argument = input.substr(firstWhitespace + 1, firstDoubleArrow - firstWhitespace - 1);
        if (!isBackground)
        {
            fileName = input.substr(firstDoubleArrow + 4, input.length());
        }
        else
        {
            fileName = input.substr(firstDoubleArrow + 4, lastNonWhitespace - firstDoubleArrow - 5);
        }
    }
    else if (firstArrow != string::npos)
    {
        argument = input.substr(firstWhitespace + 1, firstArrow - firstWhitespace - 1);
        if (!isBackground)
        {
            fileName = input.substr(firstArrow + 3, input.length());
        }
        else
        {
            fileName = input.substr(firstArrow + 3, lastNonWhitespace - firstArrow - 4);
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
            argument = input.substr(firstWhitespace + 1, lastNonWhitespace - firstWhitespace - 2);
        }
    }

    return ParsedInput(command1, argument, fileName, shouldOverwrite, isBackground);
}

bool ShellHost::doesPathExist(const string &path) const
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool ShellHost::executeCommand(const ParsedInput& input)
{
    bool foundExecutable = false;
    string extCommand;
    for (int i = 0; i < path.size() && !foundExecutable; i++)
    {
        if (doesPathExist(path[i]))
        {
            // checks if file exists in path
            for (const filesystem::directory_entry &entry : filesystem::directory_iterator(path[i]))
            {
                if (entry.path().stem().string() == input.getExecutable())
                {
                    // Prepare the command that will be passed to the OS shell via popen
                    stringstream commandBuilder;
                    commandBuilder << '"' << path[i] << '/' << input.getExecutable() << '"' << ' ' << input.getArguments();
                    
                    extCommand = commandBuilder.str();
                    foundExecutable = true;
                    break;
                }
            }
        }
    }
    // checks current directory
    for (const filesystem::directory_entry &entry : filesystem::directory_iterator(filesystem::current_path()))
    {
        if (entry.path().stem().string() == input.getExecutable() && !foundExecutable)
        {
            // Prepare the command that will be passed to the OS shell via popen
            stringstream commandBuilder;
            commandBuilder << '"' << filesystem::current_path().string() << '/' << input.getExecutable() << '"' << ' ' << input.getArguments();

            extCommand = commandBuilder.str();
            foundExecutable = true;
            break;
        }
    }

    if (!foundExecutable) return false;

    // Create a new thread on the heap and store a pointer to it, if the thread is in the background
    // Otherwise create the thread and immediately block until it exits
    if (input.shouldRunInBackground())
    {
        if (input.getOutputFile().empty())
        {
            backgroundThreads.push_back(new thread(
                ExecutionUtils::executeCommandWithConsoleOutput,
                extCommand
            ));
        }
        else
        {
            backgroundThreads.push_back(new thread(
                ExecutionUtils::executeCommandWithFileOutput,
                extCommand,
                input.getOutputFile(),
                input.shouldOverwriteOutputFile()
            ));
        }
    }
    else
    {
        if (input.getOutputFile().empty())
        {
            thread commandThread(
                ExecutionUtils::executeCommandWithConsoleOutput,
                extCommand
            );
            commandThread.join();
        }
        else
        {
            thread commandThread(
                ExecutionUtils::executeCommandWithFileOutput,
                extCommand,
                input.getOutputFile(),
                input.shouldOverwriteOutputFile()
            );
            commandThread.join();
        }
    }

    return true;
}