#include "linux_parser.h"
#include "system.h"
#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <filesystem>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace fs = std::filesystem;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
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


// DONE: An example of how to read data from the filesystem
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


// BONUS: Update this to use std::filesystem
// Take 2 with std::filesystem

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  fs::path dir = kProcDirectory;
for (const auto& entry : fs::directory_iterator(dir)){
  // while (fs::exists(entry)) {
    // Is this a directory?
    if (fs::is_directory(dir)) {
      // Is every character of the name a digit?
      std::string filename = entry.path().filename().string();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  float memAvailable, memTotal;
  int found {0};
  if (stream.is_open()) {
    string identifier, value;
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> identifier >> value;
        if(identifier == "MemAvailable:") {  
            memAvailable = std::stof(value);
            found++;
        }
        if(identifier == "MemTotal:") {  
            memTotal = std::stof(value);
            found++;
        }
        if (found >= 2){break;}
}
  }

  return (1 - memAvailable/memTotal); }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  return uptime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return Pids().size(); }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
