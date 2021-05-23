#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"
#include "format.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    processes_.clear();
    vector<int> pid_ids=LinuxParser::Pids();
    for (int pid:pid_ids) {
        processes_.emplace_back(pid); //emplace_back(pid) automatically calls the constructor
    }
    std::sort(processes_.begin(),processes_.end());
    return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel();; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() const { return LinuxParser::UpTime(); }
