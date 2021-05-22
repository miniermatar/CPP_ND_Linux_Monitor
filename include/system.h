#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include "format.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long int UpTime() const;
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();
S
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif
