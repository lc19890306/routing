#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

class Program {
public:
  Program();
  Program(const std::string &filename);
  ~Program();
  std::istream & getline(std::string &str);
  void push_back(const std::string &stmt);
  void print();
  std::size_t size();
  std::string at(const std::size_t &i) const;
  int replace(const std::size_t &idx, const std::string &str);

protected:
  std::vector<std::string> program;

private:
  std::ifstream fin;
};

#endif	// PROGRAM_HH
