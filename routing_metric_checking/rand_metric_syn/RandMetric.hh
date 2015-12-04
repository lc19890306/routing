#ifndef RANDMETRIC_HH
#define RANDMETRIC_HH

#include "string.hh"
#include "TernaryTree.hh"

#include <iostream>
#include <list>
#include <set>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>

// typedef enum {ZERO, ONE, TWO} Ternary;

class RandMetric {
public:
  RandMetric(const tMetric &type, const std::size_t &depth, const std::size_t &max_size);
  ~RandMetric();
  std::list<std::string> getProgram() const;
  std::size_t getDepth() const;
  std::size_t getMaxSize() const;
  tMetric getMetricType() const;

private:
  TernaryTree::Tree generateTree();
  std::string generateExprTree(TernaryTree::Tree &tree, const unsigned short &argc);
  std::vector<unsigned short> decToTernary(unsigned long num, const std::size_t &size);

  std::list<std::string> program;
  std::map<unsigned long, std::vector<unsigned short> > traces;
  // std::set<unsigned long> traces;
  tMetric type;
  std::size_t depth;
  std::size_t max_size;
};

#endif	// RANDMETRIC_HH
