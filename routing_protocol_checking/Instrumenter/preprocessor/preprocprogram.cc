#include "preprocprogram.hh"

PreProcProgram::PreProcProgram(const std::string &filename) :
  Program(filename) {
  fout.open(createNewFileName(filename, ".prep"), std::ios_base::out);
}

PreProcProgram::~PreProcProgram() {
  fout.close();
}

void PreProcProgram::writeIntoFile() {
  for (auto &&stmt : program)
    fout << stmt;
}
