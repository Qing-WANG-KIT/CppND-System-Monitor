#include <string>

#include "format.h"

using std::string;
using std::to_string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    
    long hours = seconds / 3600;
    long minutes = seconds % 3600 / 60;
    long secs = seconds % 60;

    string s_hours = hours > 9 ? std::to_string(hours) : "0" + std::to_string(hours);
    string s_minutes = minutes > 9 ? ":" + std::to_string(minutes) : ":0" + std::to_string(minutes);
    string s_seconds = secs > 9 ? ":" + std::to_string(secs) : ":0" + std::to_string(secs);

    return s_hours + s_minutes + s_seconds; 
}
