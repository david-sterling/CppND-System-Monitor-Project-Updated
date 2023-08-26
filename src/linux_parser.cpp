#include "linux_parser.h"

#include <dirent.h>
#include <fmt/format.h>
#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "system.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace fs = std::filesystem;

void LinuxParser::ReadAggregatedJiffies(std::array<long, 10>& jiffies) {
  string line;
  string cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    // std::cin.ignore(3);
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> jiffies[CPUStates::kUser_] >>
        jiffies[CPUStates::kNice_] >> jiffies[CPUStates::kSystem_] >>
        jiffies[CPUStates::kIdle_] >> jiffies[CPUStates::kIOwait_] >>
        jiffies[CPUStates::kIRQ_] >> jiffies[CPUStates::kSoftIRQ_] >>
        jiffies[CPUStates::kSteal_] >> jiffies[CPUStates::kGuest_] >>
        jiffies[CPUStates::kGuestNice_];
  }
}

void LinuxParser::ReadCpuStatsPid(std::array<long, 6>& stats, int pid) {
  stats[0] = LinuxParser::UpTime();
  string line;
  string cpu;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    // std::cin.ignore(3);
    std::getline(stream, line);
    std::istringstream tokenizer(line);
    int i{1};
    string token{};

    while (tokenizer >> token) {
      if (i == 14) {  // utime
        stats[1] = stol(token);
      }
      if (i == 15) {  // stime
        stats[2] = stol(token);
      }
      if (i == 16) {  // cutime
        stats[3] = stol(token);
      }
      if (i == 17) {  // cstime
        stats[4] = stol(token);
      }
      if (i == 22) {  // startime
        stats[5] = stol(token);
        return;
      }
      i++;
    }
  }
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string os, version, kernel;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory +
                       LinuxParser::kVersionFilename);
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

void LinuxParser::Pids(vector<int>& pids) {
  fs::path dir = kProcDirectory;
  for (const auto& entry : fs::directory_iterator(dir)) {
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
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  float memAvailable, memTotal;
  int found{0};
  if (stream.is_open()) {
    string identifier, value;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> identifier >> value;
      if (identifier == "MemAvailable:") {
        memAvailable = std::stof(value);
        found++;
      }
      if (identifier == "MemTotal:") {
        memTotal = std::stof(value);
        found++;
      }
      if (found >= 2) {
        break;
      }
    }
  }

  return (1 - memAvailable / memTotal);
}

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
long LinuxParser::Jiffies() {
  long jiffies;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> jiffies >> jiffies;
  }
  return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0.0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::array<long, 10> jiffies;
  LinuxParser::ReadAggregatedJiffies(jiffies);
  return jiffies[CPUStates::kUser_] + jiffies[CPUStates::kNice_] +
         jiffies[CPUStates::kSystem_] + jiffies[CPUStates::kIRQ_] +
         jiffies[CPUStates::kSoftIRQ_];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::array<long, 10> jiffies;
  LinuxParser::ReadAggregatedJiffies(jiffies);
  return jiffies[CPUStates::kIdle_];
}

// TODO: Read and return CPU utilization
//float LinuxParser::CpuUtilization(int pid) { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  vector<int> pids{};
  Pids(pids);
  return pids.size();
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::vector<int> pids{};
  LinuxParser::Pids(pids);

  int runningProcesses{0};
  for (auto& pid : pids) {
    string line;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream tokenizer(line);
      int i{1};
      string token{};
      while (tokenizer >> token) {
        if (i == 3) {  //(3) state  %c
          if (token == "R") {
            runningProcesses++;
          };
          break;
        }
        i++;
      }
    }
  }

  return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmd);
  }

  return cmd;
}

float LinuxParser::Ram(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  float vMemSize{0.0};
  if (stream.is_open()) {
    string identifier, value;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> identifier >> value;
      if (identifier == "VmSize:") {
        vMemSize = std::stof(value);
        break;
      }
    }
  }

  return (vMemSize / (float)1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string Uid{};
  if (stream.is_open()) {
    string identifier, value;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> identifier >> Uid;
      if (identifier == "Uid:") {
        break;
      }
    }
  }

  return Uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string Uid{};
  bool found{false};
  if (stream.is_open()) {
    string identifier, value;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> identifier >> Uid;
      if (identifier == "Uid:") {
        found = true;
        break;
      }
    }
  }

  if (found) {
    return Uid;
  } else
    return "";
}

long LinuxParser::UpTime(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream tokenizer(line);
    int i{1};
    string token{};
    while (tokenizer >> token) {
      if (i == 22) {
        return stol(token) / sysconf(_SC_CLK_TCK);
      }
      i++;
    }
  }

  return 0;
}

std::string LinuxParser::FindUserName(std::string user) {
  string line;
  std::ifstream stream(kPasswordPath);
  string username, uid{};
  if (stream.is_open()) {
    string identifier, value;
    while (std::getline(stream, line)) {
      string token{};
      int i{1};
      std::istringstream ss(line);
      while (std::getline(ss, token, ':')) {
          if (i == 1) {
            username = token;
          }
          if (i == 3) {
            if (token == user) {
              return username;
            }
            else{break;}
          }
          i++;
        }
      }
    }
  

  return "Unknown";
}
