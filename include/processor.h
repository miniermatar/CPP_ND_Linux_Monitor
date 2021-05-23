#ifndef PROCESSOR_H
#define PROCESSOR_H


class Processor {
 public:
  double Utilization();

 private:
  double cpu_idle[2]={0.0,0.0};
  double cpu_nonidle[2]={0.0,0.0};
};

#endif
