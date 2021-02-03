#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <unistd.h>

// TODO: Return the aggregate CPU utilization
void Processor :: calculateTime() {
    std::string line, cpuNum;
    double user, nice, system, idle, iowait, irq, softirq, steal, guest, guestNice;
    std::ifstream file(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream filestream(line);
        filestream>>cpuNum>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guestNice;
    }
    Processor::idleTotal = idle + iowait;
    Processor::nonIdleTotal = user + nice + system + irq + softirq + steal;
}

float Processor::Utilization() {
    float prevIdleTotal = Processor::prevIdleTotal;
    float prevNonIdleTotal = Processor::prevNonIdleTotal;

    float prevTotal = prevIdleTotal + prevNonIdleTotal;

    calculateTime();

    float idleTotal = Processor::idleTotal;
    float nonIdleTotal = Processor::nonIdleTotal;

    float total = idleTotal + nonIdleTotal;

    float totalD = total - prevTotal;
    float idleD = idleTotal - prevIdleTotal;

    Processor::prevIdleTotal = idleTotal;
    Processor::prevNonIdleTotal = nonIdleTotal;

    return (totalD - idleD) / totalD;
}