#ifndef CHKPTPROGRAM_HH
#define CHKPTPROGRAM_HH

#include "program.hh"
#include "string.hh"
#include "psprogram.hh"

#include <map>
#include <set>
#include <algorithm>

class ChkPtProgram : public Program {
public:
  ChkPtProgram(const std::vector<std::set<std::string> > &vars,
	       const std::map<std::string, std::string> &argList,
	       const std::map<std::size_t, std::set<std::string> > &cmpChkPtVars);
  ~ChkPtProgram();
  // bool equals(const std::set<std::string> &lhs, const std::set<std::string> &rhs);
  void insertChkPtFuncs();
  std::string chkPtStmt(const std::string &stmt);
  void writeIntoFile();

private:
  std::ofstream fout;
  std::vector<std::set<std::string> > vars;
  std::map<std::string, std::string> argList;
  std::map<std::size_t, std::set<std::string> > cmpChkPtVars;
};

#endif	// CHKPTPROGRAM_HH
