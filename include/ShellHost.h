#ifndef SHELL_HOST_H
#define SHELL_HOST_H

#include <string>
#include <vector>

class ShellHost
{
public:
    ShellHost(std::string user, std::string host, std::vector<std::string> path);
    void operator()();
private:
    std::string user;
    std::string host;
    std::vector<std::string> path;
};

#endif
