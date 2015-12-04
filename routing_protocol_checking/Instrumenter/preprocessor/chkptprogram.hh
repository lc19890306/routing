#ifndef CHKPTPROGRAM_HH
#define CHKPTPROGRAM_HH

#include "program.hh"
#include "string.hh"

class ChkPtProgram : public Program {
public:
  ChkPtProgram(const unsigned &n);
  ~ChkPtProgram();
  void writeIntoFile();

private:
  std::ofstream fout;
};

#endif	// CHKPTPROGRAM_HH
