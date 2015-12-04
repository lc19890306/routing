#ifndef TESTPROGRAM_HH
#define TESTPROGRAM_HH

#include "RandMetric.hh"

#include <fstream>
#include <string>
// #include <vector>
#include <list>

class TestProgram {
public:
  TestProgram(const RandMetric &metric);
  ~TestProgram();
  void print();
  void writeIntoFile();

private:
  void insertTestStmts();
  void left_isotonicity_check(std::string &&res0, std::string &&res1, std::string &&res4, std::string &&res5);
  void right_isotonicity_check(std::string &&res0, std::string &&res1, std::string &&res2, std::string &&res3);
  void left_monotonicity_check(std::string &&res0, std::string &&res4);
  void right_monotonicity_check(std::string &&res0, std::string &&res2);
  void odd_symmetry_check();
  void transitivity_check(const std::string &max_size_str);
  void strict_order_check();

  std::ofstream fout;
  // std::vector<std::string> program;

  std::list<std::string> program;
  std::size_t max_size;
  tMetric type;
};

#endif	// TESTPROGRAM_HH
