#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
    int hours, minutes;
    hours = seconds / 3600;    
    minutes = (seconds % 3600) / 60;
    seconds = (seconds % 3600) % 60;    
    
    char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
    return buffer;
}