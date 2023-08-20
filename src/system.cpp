#include "system.h"

#include <unistd.h>
#include <iostream>

#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

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
vector<Process>& System::Processes() { 
  
  std::vector<int> pids {};
  //processes_.clear();

  LinuxParser::Pids(pids);

  // Update processes vector with new processes
  for (unsigned long i =0; i < pids.size(); i++){
    //We can use binary_search to speed up since vector is ordered
    if ( std::find(processes_.begin(), processes_.end(), Process(pids[i])) == processes_.end() ){
    processes_.push_back(Process((pids[i])));
    }
    
  }  

  // Delete terminated processes

  for (unsigned long i =0; i < processes_.size(); i++){
    //We can use binary_search to speed up since vector is ordered
    if ( std::find(pids.begin(), pids.end(), processes_[i].Pid()) == pids.end() ){
    //processes_.push_back(Process((pids[i])));
    processes_.erase(processes_.begin()+i);
    }
    
  }  


  std::sort(processes_.begin(), processes_.end());
  return processes_;

 }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 

  return LinuxParser::Kernel();
   }

//TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization();
}

//TODO: Return the operating system name
std::string System::OperatingSystem() {  return LinuxParser::OperatingSystem(); }

//TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

//TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

//TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

std::string System::FindUser(std::string uid){

  if (users_.find(uid) == users_.end()){
    return "Unknown" ;}
  else{
  return users_[uid];
  }
}