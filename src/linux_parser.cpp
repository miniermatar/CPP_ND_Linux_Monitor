#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

std::string LinuxParser::parameter(std::string filepath, std::string key_name) {
    string line;
    string key,key_value;
    string value;
    std::ifstream filestream(filepath);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> value;
          if (strncmp(key.c_str(),key_name.c_str(),key_name.length())==0) {
            key_value=value;
            break;
          }
        }
      }
    return key_value;
}

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
    float MemTotal=0;
    float MemFree=0;
    std::string value;
    value=parameter(kProcDirectory + kMeminfoFilename,"MemTotal");
    value.erase(std::remove(value.begin(), value.end(), 'k'),value.end());
    value.erase(std::remove(value.begin(), value.end(), 'B'),value.end());
    if (value!="")
      MemTotal=stof(value);
    value=parameter(kProcDirectory + kMeminfoFilename,"MemFree");
    value.erase(std::remove(value.begin(), value.end(), 'k'),value.end());
    value.erase(std::remove(value.begin(), value.end(), 'B'),value.end());
    if (value!="")
      MemFree=stof(value);
    return ((MemTotal-MemFree)/(MemTotal));
}

long LinuxParser::UpTime() {

    long uptime=0;
    string line, uptime_string;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> uptime_string;
    if (uptime_string!="")
      uptime = std::stof(uptime_string);
    }
    return uptime; }


int LinuxParser::TotalProcesses() {
    std::string value = parameter(kProcDirectory + kStatFilename,"processes");
    return std::stoi(value);
}

int LinuxParser::RunningProcesses() {
    std::string value = parameter(kProcDirectory + kStatFilename,"procs_running");
    return std::stoi(value);
}

string LinuxParser::Command(int pid) {
    string line;
    string key,key_value;
    string value;
    std::ifstream filestream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
    }
    return line;
}
// In this application, I am measuring the amount of physical memory needed for the process (VmData) instead of the virtual memory (VmSize)
string LinuxParser::Ram(int pid) {
    float vm=0;
    std::string value;
    value=parameter(kProcDirectory + std::to_string(pid) + kStatusFilename,"VmData");
    value.erase(std::remove(value.begin(), value.end(), 'k'),value.end());
    value.erase(std::remove(value.begin(), value.end(), 'B'),value.end());
    if (value!="")
        vm=stof(value)*0.001; //converting to MB
    return std::to_string (static_cast<int>(vm));
}

string LinuxParser::Uid(int pid) {
    std::string value;
    value=parameter(kProcDirectory + std::to_string(pid) + kStatusFilename,"Uid");
    return value;
}

string LinuxParser::User(int pid) {
    std::string uid = LinuxParser::Uid(pid);
    string line, user, username, password, uid_code;
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> user >> password >> uid_code;
          if (strncmp(uid_code.c_str(),uid.c_str(),uid.length())==0) {
            break;
          }
      }
    }
    return user;
}

long LinuxParser::UpTime(int pid) {
    float uptime_sec=0;
    string line, uptime;
    std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        int i=0;
        while (linestream) {
            i++;
            linestream >> uptime;
            if (i==22)
                break;
        }
    }
    if (uptime!="") {
        std::string kernel = Kernel();
        float version = std::stof(kernel.erase(3,kernel.length()));
        if (version<=2.6) {
                uptime_sec = UpTime()-std::stof(uptime)/60;
        } else {
                uptime_sec = UpTime()-std::stof(uptime)/sysconf(_SC_CLK_TCK);
        }
    }
        return static_cast<long>(uptime_sec);
}

float LinuxParser::CpuUtilization(int pid) {
    float utime,stime,cutime,cstime,starttime;
    string line, value;
    std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        int i=0;
        while (linestream) {
            i++;
            linestream >> value;
            if (value=="")
                value="0";
            switch (i) {
                case 14:
                    utime= stof(value);
                    break;
                case 15:
                    stime=stof(value);
                    break;
                case 16:
                    cutime=stof(value);
                    break;
                case 17:
                    cstime=stof(value);
                    break;
                case 22:
                    starttime=stof(value);
                    break;
                default:
                    break;
            }
        }
    }
    float sys_uptime=(float)LinuxParser::UpTime();
    float total_time_sec=(utime+stime+cutime+cstime)/sysconf(_SC_CLK_TCK);
    float pid_uptime_sec=sys_uptime-(starttime/sysconf(_SC_CLK_TCK));
    return total_time_sec/pid_uptime_sec;
}
