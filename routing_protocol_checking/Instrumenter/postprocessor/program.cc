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

std::size_t Program::size() {
  return program.size();
}

std::string Program::at(const std::size_t &i) const {
  return program[i];
}

int Program::replace(const std::size_t &idx, const std::string &str) {
  // assert(program.size() > idx && "Error: out of bound!");
  try {
    program[idx] = str;
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl << "Error: out of bound!\n";
    return -1;
  }
  return 0;
}
