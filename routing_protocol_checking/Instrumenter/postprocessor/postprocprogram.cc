#include "postprocprogram.hh"

PostProcProgram::PostProcProgram(const std::string &filename) :
  Program(filename) {
  fout.open(createNewFileName(filename, ".prep"), std::ios_base::out);
}

PostProcProgram::~PostProcProgram() {
  fout.close();
}

void PostProcProgram::writeIntoFile() {
  for (auto &&stmt : program)
    fout << stmt;
}
