#include "psprogram.hh"

PSProgram::PSProgram() {
  fout.open("ProgramStates.hh", std::ios_base::out);
  program.push_back("#ifndef PROGRAMSTATES_HH\n");
  program.push_back("#define PROGRAMSTATES_HH\n");
  program.push_back("\n");
  program.push_back("#include \"ProgramState.hh\"\n");
  program.push_back("\n");
  program.push_back("#include <cstring>\n");
  program.push_back("\n");
}

PSProgram::~PSProgram() {
  fout.close();
}

std::string PSProgram::addPSTy(const std::map<std::string, std::string> &psVars) {
  std::string ps_idx(itoa(psCounter));

  program.push_back("class ProgramState");
  program.push_back(ps_idx);
  program.push_back(" : public ProgramState {\n");
  program.push_back("public:\n");
  program.push_back("ProgramState");
  program.push_back(ps_idx);
  program.push_back("(");

  std::string ret("ProgramState");
  ret += ps_idx;
  ret += " *ps = new ProgramState";
  ret += ps_idx;
  ret += "(";
  for (auto var_pair : psVars) {
    program.push_back("const ");
    program.push_back(var_pair.second);
    program.push_back(" ");
    // std::cout << var_pair.second << " " << var_pair.first << std::endl;
    auto pos_lsqrBracket(var_pair.first.find("["));
    if (pos_lsqrBracket != std::string::npos)
      ret += var_pair.first.substr(0, pos_lsqrBracket);
    else {
      program.push_back("&");
      ret += var_pair.first;
    }
    program.push_back(var_pair.first);
    program.push_back(",\n");
    ret += ", ";
  }
  program[program.size() - 1] = ") {\n";
  ret.replace(ret.size() - strlen(", "), strlen(");\n"), ");\n");

  for (auto var_pair : psVars) {
    auto pos_lsqrBracket(var_pair.first.find("["));
    if (pos_lsqrBracket != std::string::npos) {
      std::string arrayName(var_pair.first.substr(0, pos_lsqrBracket));
      program.push_back("std::memcpy(");
      program.push_back("this->");
      program.push_back(arrayName);
      program.push_back(", ");
      program.push_back(arrayName);
      program.push_back(", ");
      program.push_back("sizeof(");
      program.push_back(var_pair.second);
      program.push_back(")");

      while (pos_lsqrBracket != std::string::npos) {
	program.push_back(" * ");
  	auto pos_rsqrBracket(matchSqrBracket(var_pair.first, pos_lsqrBracket));
  	program.push_back(var_pair.first.substr(pos_lsqrBracket + 1, pos_rsqrBracket - pos_lsqrBracket - 1));
	pos_lsqrBracket = var_pair.first.find("[", pos_rsqrBracket + 1);
      }

      program.push_back(");\n");
    } else {
      program.push_back("this->");
      program.push_back(var_pair.first);
      program.push_back(" = ");
      program.push_back(var_pair.first);
      program.push_back(";\n");
    }
  }
  program.push_back("}\n\n");

  for (auto var_pair : psVars) {
    program.push_back(var_pair.second);
    program.push_back(" ");
    program.push_back(var_pair.first);
    program.push_back(";\n");
  }

  program.push_back("};\n");

  return ret;
}

void PSProgram::writeIntoFile() {
  program.push_back("\n");
  program.push_back("#endif\n");
  for (auto &&stmt : program)
    fout << stmt;
}

std::size_t PSProgram::psCounter = 0;
