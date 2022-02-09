#ifndef EXECUTION_UTILS_H
#define EXECUTION_UTILS_H

#include <string>

namespace ExecutionUtils
{
    std::string executeCommand(const std::string& command);
    void executeCommandWithConsoleOutput(const std::string& command);
    void executeCommandWithFileOutput(const std::string& command, const std::string& filePath, bool shouldOverwrite);
}

#endif
