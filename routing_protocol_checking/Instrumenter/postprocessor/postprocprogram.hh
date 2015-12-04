#ifndef POSTPROCPROGRAM_HH
#define POSTPROCPROGRAM_HH

#include "program.hh"
#include "string.hh"

#include <vector>
#include <map>

class PostProcProgram : public Program {
public:
  PostProcProgram(const std::string &filename);
  ~PostProcProgram();
  void writeIntoFile();

private:
  std::ofstream fout;
};

#endif	// POSTPROCPROGRAM_HH
