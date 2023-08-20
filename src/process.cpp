#include "process.h"

#include <fmt/format.h>
#include <format.h>
#include <linux_parser.h>
#include <unistd.h>
#include <system.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

using namespace LinuxParser;

Process::Process(int mypid) : pid_(mypid) {
  prevStat_.fill(0);
  mem_ = 0.0;
  cpu_ = 0.0;
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long sysTicks = sysconf(_SC_CLK_TCK);
  std::array<long, 6> currentStats;
  ReadCpuStatsPid(currentStats, pid_);

  long currentTime =
      currentStats[PidStates::kUtime_] + currentStats[PidStates::kStime_] +
      currentStats[PidStates::kCstime_] + currentStats[PidStates::kCutime_];
  long lastTime =
      prevStat_[PidStates::kUtime_] + prevStat_[PidStates::kStime_] +
      prevStat_[PidStates::kCstime_] + prevStat_[PidStates::kCutime_];
  // long seconds = currentStats[PidStates::kUptime_] -
  // prevStat_[PidStates::kUptime_] -
  //                 (currentStats[PidStates::kStartime_] / sysTicks);
  long seconds {0};
  if (prevStat_[PidStates::kUptime_] == 0 ){
    seconds =
      currentStats[PidStates::kUptime_] - (currentStats[PidStates::kStartime_] / sysTicks);
  }
  else {
    seconds =
      currentStats[PidStates::kUptime_] - prevStat_[PidStates::kUptime_];
  }
  long deltaTime = currentTime - lastTime;
  std::copy(std::begin(currentStats), std::end(currentStats),
            std::begin(prevStat_));
  if (deltaTime == 0 || seconds == 0) {
    return 0.0;
  } else {
    float usage = ((float)deltaTime / (float)sysTicks) / (float)seconds;
    return usage;
  }
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() {
  mem_ = LinuxParser::Ram(pid_);
  string formatted = fmt::format("{: <08.02f}", mem_);
  return formatted;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  std::string user = LinuxParser::User(pid_);
  
  return LinuxParser::FindUserName(user);
 }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return (a.cpu_ < cpu_); }

bool Process::operator!=(Process const& p1) { return (p1.pid_ != pid_); }

bool Process::operator==(Process const& p1) { return (p1.pid_ == pid_); }