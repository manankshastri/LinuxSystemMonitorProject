#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hours, minutes;
    hours = seconds / 3600;
    minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;

    string h = std::to_string(hours);
    string m = std::to_string(minutes);
    string s = std::to_string(seconds);

    h.insert(0, 2 - h.length(), '0');
    m.insert(0, 2 - m.length(), '0');
    s.insert(0, 2 - s.length(), '0');

    //return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    return h + ":" + m + ":" + s;
}