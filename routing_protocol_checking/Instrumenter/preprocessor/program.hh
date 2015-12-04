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

protected:
  std::vector<std::string> program;

private:
  std::ifstream fin;
};

#endif	// PROGRAM_HH
