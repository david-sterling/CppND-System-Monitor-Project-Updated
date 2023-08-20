#include "processor.h"

#include <algorithm>

#include "linux_parser.h"

using namespace LinuxParser;

// TODO: Return the aggregate CPU utilization
float Processor::AverageUtilization() { return 0.0; }

float Processor::InstantUtilization() {
  std::array<long, 10> jiffies{};
  LinuxParser::ReadAggregatedJiffies(jiffies);

  float usage = 1 - ((float)DeltaJiffies(jiffies, CPUStates::kIdle_) /
                     ((float)DeltaJiffies(jiffies, CPUStates::kUser_) +
                      (float)DeltaJiffies(jiffies, CPUStates::kNice_) +
                      (float)DeltaJiffies(jiffies, CPUStates::kSystem_) +
                      (float)DeltaJiffies(jiffies, CPUStates::kIdle_) +
                      (float)DeltaJiffies(jiffies, CPUStates::kIRQ_) +
                      (float)DeltaJiffies(jiffies, CPUStates::kSoftIRQ_)));

  std::copy(std::begin(jiffies), std::end(jiffies), std::begin(lastJiffies));

  return usage;
}

long Processor::DeltaJiffies(std::array<long, 10>& jiffies, int CPUState) {
  if (CPUState > 0 && CPUState < 10) {
    return jiffies[CPUState] - lastJiffies[CPUState];
  }

  else {
    return 0;
  }
}