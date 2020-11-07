#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // See src/processor.cpp

  // TDeclare any necessary private members
 private:
 long activejiffies_n_1 = 0;
 long totaljiffies_n_1 = 0;
};

#endif