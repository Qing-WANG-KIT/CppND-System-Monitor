#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  Process::pid_=pid;
  Process::proc_cpuutil_ = Process::CpuUtilization();
} 
// Return this process's ID
int Process::Pid() { return pid_ ; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return LinuxParser::ActiveJiffies(pid_); }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return proc_cpuutil_>a.proc_cpuutil_; }