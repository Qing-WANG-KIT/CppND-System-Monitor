#include "processor.h"
#include "linux_parser.h"
#include "thread"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  long activejiffies = LinuxParser::ActiveJiffies();
  long totaljiffies = LinuxParser::Jiffies();
  float utilize = 0.0;
  if(totaljiffies != totaljiffies_n_1) {
    utilize = (float) (activejiffies - activejiffies_n_1) / (float) (totaljiffies - totaljiffies_n_1);
  }
  else
  {
    utilize = (float) activejiffies / (float) totaljiffies;
  }
  activejiffies_n_1 = activejiffies;
  totaljiffies_n_1 = totaljiffies;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return utilize;
}