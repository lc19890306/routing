#include "RandMetric.hh"
#include "TestProgram.hh"
#include "Options.hh"

#include <iostream>
#include <climits>

int main(int argc, char **argv) {
  Options options(argc, argv);
  std::size_t depth(options.getDepth()), max_size(options.getMaxSize());
  tMetric type(options.getMetricType());
  RandMetric metric(type, depth, max_size);
  TestProgram program(metric);
  // program.print();
  // program.writeIntoFile();

  return 0;
}
