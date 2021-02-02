#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    string line, key, size;
    float value;
    int counter = 0;

    std::vector<float> memory;

    std::ifstream file(kProcDirectory + kMeminfoFilename);
//    if (file.is_open()) {
//        while (std::getline(file, line) && counter++ != 2) {
//            std::istringstream filestream(line);
//            filestream>>key>>value>>size;
//            memory.push_back(value);
//        }
//    }
//
//    float memFree = memory.back();
//    memory.pop_back();
//    float memTotal = memory.back();
//
//    return (memTotal - memFree) / memTotal;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream filestream(line);
            while(filestream>>key>>value>>size) {
                if (key.compare("MemTotal:") == 0)
                    memory.push_back(value);

                if (key.compare("MemFree:") == 0)
                    memory.push_back(value);

                if (key.compare("Buffers:") == 0)
                    memory.push_back(value);

                if (key.compare("Cached:") == 0)
                    memory.push_back(value);

                if (key.compare("SReclaimable:") == 0)
                    memory.push_back(value);
            }

        }
    }

    file.close();

    float SReclaimable = memory.back();
    memory.pop_back();
    float Cached = memory.back();
    memory.pop_back();
    float Buffers = memory.back();
    memory.pop_back();
    float MemFree = memory.back();
    memory.pop_back();
    float MemTotal = memory.back();

    return (MemTotal - MemFree - Buffers - Cached - SReclaimable) / MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    string line;
    long uptime = 0;
    long idle = 0;
    std::ifstream file(kProcDirectory + kUptimeFilename);
    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream linestream(line);
        linestream>>uptime>>idle;
    }
    file.close();
    long total = uptime + idle;
    return total;
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
int LinuxParser::TotalProcesses() {
    string line, key;
    int value;
    std::ifstream file(kProcDirectory + kStatFilename);
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream filestream(line);
            while (filestream>>key>>value) {
                if (key.compare("processes") == 0) {
                    file.close();
                    return value;
                }
            }
        }
    }
    return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, key;
    int value;
    std::ifstream file(kProcDirectory + kStatFilename);
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream filestream(line);
            while (filestream>>key>>value) {
                if (key.compare("procs_running") == 0) {
                    file.close();
                    return value;
                }
            }
        }
    }
    return 0;
}

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