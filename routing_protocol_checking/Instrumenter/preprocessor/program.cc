#include "program.hh"

Program::Program() {}

Program::Program(const std::string &filename) {
  fin.open(filename, std::ios_base::in);
}

Program::~Program() {
  fin.close();
}

std::istream & Program::getline(std::string &str) {
  return std::getline(fin, str);
}

void Program::push_back(const std::string &stmt) {
  program.push_back(stmt);
}

void Program::print() {
  for (auto &&stmt : program)
    std::cout << stmt;
}
