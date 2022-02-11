#ifndef SHELL_HOST_H
#define SHELL_HOST_H

#include <string>
#include <vector>
#include <thread>
#include "./ParsedInput.h"

class ShellHost
{
public:
    ShellHost(std::string user, std::string host, std::vector<std::string> path);
    ~ShellHost();
    void operator()();
private:
    ParsedInput ParseUserInput(const std::string& input) const;
private:
    std::string user;
    std::string host;
    std::vector<std::string> path;
    std::vector<std::thread*> backgroundThreads;
};

#endif
