#ifndef PSPROGRAM_HH
#define PSPROGRAM_HH

#include "program.hh"
#include "string.hh"

#include <iostream>
#include <map>

class PSProgram : public Program {
public:
  PSProgram();
  ~PSProgram();
  std::string addPSTy(const std::map<std::string, std::string> &psVars);
  void writeIntoFile();

private:
  std::ofstream fout;
  static std::size_t psCounter;
};

#endif	// PSPROGRAM_HH
