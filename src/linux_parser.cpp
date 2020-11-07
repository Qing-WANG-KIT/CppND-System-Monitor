#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "PRETTY_NAME") {
        std::replace(value.begin(), value.end(), '_', ' ');
        return value;
      }
    }
  }
  return "Not valid";
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float mem_toal, mem_free;
  string line, key;
  int value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if (key == "MemTotal:") mem_toal = value;
        if (key == "MemFree:") mem_free = value;
      }
    }
  }
  return ((mem_toal - mem_free) / mem_toal); 
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line, uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return std::stoi(uptime);
  }
  return 0;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  int i = 1;
  long sum = 0;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value) {
      if(i >= 14 && i <= 17) {
        sum += stoi(value);
      }
      if(i > 17) {
        break;
      }
      i++;
    }
  }
  return sum;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line, cpu, value;
  vector<long> jiffies;
  long sum = 0;
  int i = 1;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
  	std::getline(stream, line);
  	std::istringstream linestream(line);
  	linestream >> cpu;
    if(cpu == "cpu") {
  	  while(linestream >> value) {
        if((i >= 1 && i <= 3) || (i >= 6 && i <= 8)) {
          sum += stoi(value);
        }
        i++;
  	  }
      return sum;
    }
  }
  return sum;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line, cpu, value;
  long sum = 0;
  int i = 1;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
  	std::getline(stream, line);
  	std::istringstream linestream(line);
  	linestream >> cpu;
    if(cpu == "cpu") {
  	  while(linestream >> value) {
        if(i == 4 || i == 5) {
          sum += stoi(value);
        }
        i++;
  	  }
      return sum;
    }
  }
  return sum;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, value;
  vector<string> cpu;
  int i = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value) {
      if(i >= 1 && i <= 10) {
        cpu.push_back(value);
      }
    }
  }
  return cpu;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, name, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> value;
      if(name == "processes") {
        return std::stoi(value);
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, name, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> value;
      if(name == "procs_running") {
        return std::stoi(value);
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string{};
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, name, value;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> value;
      if(name == "VmSize:") {
        return to_string(stoi(value) / 1000);
      }
    }
  }
  return string{};
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, name, value, uid;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> value;
      if(name == "Uid:") {
        return value;
      }
    }
  }
  return string{};
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, name, x, value_1, value_2;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> x >> value_1 >> value_2;
      if(value_1 == LinuxParser::Uid(pid)) {
        return name;
      }
    }
  }
  return string{};
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, value;
  int i = 1;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value) {
      if(i == 22) {
        return (long) ((float) LinuxParser::UpTime() - stof(value) / sysconf(_SC_CLK_TCK));
      }
      i++;
    }
  }
  return 0;
}