#ifndef PROCESSOR_H
#define PROCESSOR_H


class Processor {
 public:
  float Utilization();

 private:
  float cpu_idle[2]={0.0,0.0};
  float cpu_nonidle[2]={0.0,0.0};
};

#endif
