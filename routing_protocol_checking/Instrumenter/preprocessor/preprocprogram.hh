#ifndef PREPROCPROGRAM_HH
#define PREPROCPROGRAM_HH

#include "program.hh"
#include "string.hh"

class PreProcProgram : public Program {
public:
  PreProcProgram(const std::string &filename);
  ~PreProcProgram();
  void writeIntoFile();

private:
  std::ofstream fout;
};

#endif	// PREPROCPROGRAM_HH
