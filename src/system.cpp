#include "system.h"

#include <unistd.h>
#include <iostream>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  string os, version, kernel;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
   }

//TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization();
}

//TODO: Return the operating system name
std::string System::OperatingSystem() { 
  string os, version, kernel;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return os;
   }

//TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

//TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

//TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }