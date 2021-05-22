#include "processor.h"
#include "linux_parser.h"
using std::stof;
using std::string;

//Return CPU utilization for every refresh cycle
float Processor::Utilization() {
    string line;
    string key,user,nice,system,idle,iowait,irq,softirq,steal,guest,guestnice;
    float total[2]={0.0,0.0};
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
        cpu_idle[0]=cpu_idle[1];
        cpu_idle[1]= stof(idle)+stof(iowait);
        cpu_nonidle[0]=cpu_nonidle[1];
        cpu_nonidle[1]=stof(user)+stof(nice)+stof(system)+stof(irq)+stof(softirq)+stof(steal);
    }
    for (int i=0;i<=1;i++)
        total[i]=cpu_idle[i]+cpu_nonidle[i];

    float total_diff=total[1]-total[0];
    float idle_diff=cpu_idle[1]-cpu_idle[0];


    return (total_diff-idle_diff)/total_diff;
}
