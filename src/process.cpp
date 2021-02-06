#include "process.h"
#include "linux_parser.h"
#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::CpuUtilization() const {
    long utime, stime, cutime, cstime, starttime, totalTime;
    float seconds;

    auto cpuTime = LinuxParser::CpuUtilization(_pid);

    if (!cpuTime.empty()) {
        starttime = cpuTime.back();
        cpuTime.pop_back();

        cstime = cpuTime.back();
        cpuTime.pop_back();

        cutime = cpuTime.back();
        cpuTime.pop_back();

        stime = cpuTime.back();
        cpuTime.pop_back();

        utime = cpuTime.back();
    }

    cpuTime.clear();

    long uptime = LinuxParser::UpTime();

    totalTime = utime + stime + cutime + cstime;
    seconds = (float)uptime - (float)(starttime / sysconf(_SC_CLK_TCK));

    return (float) totalTime / (float) sysconf(_SC_CLK_TCK) / seconds;

}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid());}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid());}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(Pid());}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() > a.CpuUtilization();
}