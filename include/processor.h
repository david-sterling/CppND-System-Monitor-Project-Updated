#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <array>

class Processor {
 public:
  float AverageUtilization();  // TODO: See src/processor.cpp
  float InstantUtilization();  //Aggregated instant CPU usage

  // TODO: Declare any necessary private members
 private:
 std::array<long ,10> lastJiffies {};
 long DeltaJiffies(std::array<long, 10>& jiffies, int CPUState);

};

#endif