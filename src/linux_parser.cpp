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
// https://access.redhat.com/solutions/406773
float LinuxParser::MemoryUtilization() {
    string line, key, size;
    float value;

    std::vector<float> memory;

    std::ifstream file(kProcDirectory + kMeminfoFilename);

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
    long upTime = 0;
    long idleTime = 0;
    std::ifstream file(kProcDirectory + kUptimeFilename);
    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream linestream(line);
        linestream>>upTime>>idleTime;
        file.close();
        return upTime;
    }
    return upTime;
}

/*
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; } */

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization(int pid) {
    string line, comm;
    char state;
    long ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt;
    long cmajflt, utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue, starttime;

    std::vector<long> cpuTime;

    std::ifstream file(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream filestream(line);
        filestream>>pid>>comm>>state>>ppid>>pgrp>>session>>tty_nr>>tpgid>>flags>>minflt>>cminflt>>majflt>>
                    cmajflt>>utime>>stime>>cutime>>cstime>>priority>>nice>>num_threads>>itrealvalue>>starttime;

        cpuTime.push_back(utime);
        cpuTime.push_back(stime);
        cpuTime.push_back(cutime);
        cpuTime.push_back(cstime);
        cpuTime.push_back(starttime);
        file.close();
    }
    return cpuTime;
}

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
string LinuxParser::Command(int pid) {
    string line, cmd;

    std::ifstream file(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream filestream(line);
        filestream>>cmd;
        file.close();
        return line;
    }

    return "Not Found!!";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string line, key, value, size;

    std::ifstream file(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (file.is_open()) {
        while(std::getline(file, line)) {
            std::istringstream filestream(line);
            while(filestream>>key>>value) {
                if (key.compare("VmSize:") == 0) {
                    file.close();
                    return std::to_string(std::stoi(value) / 1024);
                }
            }
        }
    }
    return value;

}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line, key;
    string value1;

    std::ifstream file(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream filestream(line);
            while (filestream>>key>>value1) {
                if (key.compare("Uid:") == 0) {
                    file.close();
                    return value1;
                }
            }
        }
    }
    return value1;
}

//
//// TODO: Read and return the user associated with a process
//// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string line, user, x, uid;

    std::ifstream file(kPasswordPath);

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');

            std::istringstream filestream(line);
            filestream>>user>>x>>uid;

            if (uid == LinuxParser::Uid(pid)) {
                file.close();
                return user;
            }
        }
    }
    return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string line, comm;
    char state;
    long ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt;
    long cmajflt, utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue;
    long starttime = 0;

    std::ifstream file(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream filestream(line);
        filestream>>pid>>comm>>state>>ppid>>pgrp>>session>>tty_nr>>tpgid>>flags>>minflt>>cminflt>>majflt>>
                  cmajflt>>utime>>stime>>cutime>>cstime>>priority>>nice>>num_threads>>itrealvalue>>starttime;
    }
    file.close();
    return (starttime / sysconf(_SC_CLK_TCK));
}