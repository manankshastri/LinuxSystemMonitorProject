#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  void calculateTime();

  // TODO: Declare any necessary private members
 private:
    float prevIdleTotal{0};
    float prevNonIdleTotal{0};
    float idleTotal;
    float nonIdleTotal;
};

#endif