#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

Process::Process (int pid): pid_(pid) {
    user_=LinuxParser::User(pid);
    cpu_u_=LinuxParser::CpuUtilization(pid);
    ram_=LinuxParser::Ram(pid);
    uptime_=LinuxParser::UpTime(pid);
    command_= LinuxParser::Command(pid);
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpu_u_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }

//Overloading the < operatior to compare based on Ram memory.
bool Process::operator<(Process const& a) const {
    if (stof(ram_) > stof(a.ram_))
        return true;
    else
        return false;
}
