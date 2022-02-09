#include "include/ExecutionUtils.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

namespace ExecutionUtils
{
    std::string executeCommand(const std::string& command)
    {
        char readBuffer[128];
        std::string writeBuffer;
        FILE* pipe;

        #ifdef _WIN32
            pipe = _popen(command.c_str(), "rt");
        #else
            pipe = popen(command.c_str(), "r");
        #endif

        if (pipe != NULL)
        {
            while (fgets(readBuffer, 128, pipe))
            {
                writeBuffer += readBuffer;
            }

            #ifdef _WIN32
                _pclose(pipe);
            #else
                pclose(pipe);
            #endif
        }

        return writeBuffer;
    }

    void executeCommandWithConsoleOutput(const std::string& command)
    {
        std::cout << executeCommand(command) << std::endl;
    }

    void executeCommandWithFileOutput(const std::string& command, const std::string& filePath, bool shouldOverwrite)
    {
        auto output = executeCommand(command);
        std::ofstream outFile(filePath, shouldOverwrite ? std::ios_base::trunc : std::ios_base::app);
        if (outFile.is_open())
        {
            outFile << output;
        }
    }
}
