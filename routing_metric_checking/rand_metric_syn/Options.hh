#ifndef OPTIONS_HH
#define OPTIONS_HH

#include "string.hh"
#include "definitions.hh"

#include <iostream>
#include <climits>

class Options {
public:
  Options(int argc, char **argv);
  std::size_t getDepth() const;
  std::size_t getMaxSize() const;
  tMetric getMetricType() const;

private:
  std::size_t depth;
  std::size_t max_size;
  tMetric type;
};

#endif	// OPTIONS_HH
