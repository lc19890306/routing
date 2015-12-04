#include "chkptprogram.hh"

ChkPtProgram::ChkPtProgram(const unsigned &n) {
  fout.open("chkpt.hh", std::ios_base::out);
  program.push_back("#ifndef CHKPT_HH\n");
  program.push_back("#define CHKPT_HH\n");
  program.push_back("\n");
  program.push_back("#include \"path.h\"\n");
  program.push_back("\n");
  for (int i = 0; i != n; ++i) {
    std::string stmt("void checkpoint");
    stmt += itoa(i);
    stmt += "(const Path<int> &path) {}\n";
    program.push_back(stmt);
  }
  program.push_back("\n");
  program.push_back("#endif\n");
}

ChkPtProgram::~ChkPtProgram() {
  fout.close();
}

void ChkPtProgram::writeIntoFile() {
  for (auto &&stmt : program)
    fout << stmt;
}
