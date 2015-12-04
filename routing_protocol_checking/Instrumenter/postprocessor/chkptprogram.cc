#include "chkptprogram.hh"

ChkPtProgram::ChkPtProgram(const std::vector<std::set<std::string> > &vars,
			   const std::map<std::string, std::string> &argList,
			   const std::map<std::size_t, std::set<std::string> > &cmpChkPtVars) :
  vars(vars), argList(argList), cmpChkPtVars(cmpChkPtVars) {
  fout.open("checkpoint.hh", std::ios_base::out);
  program.push_back("#ifndef CHECKPOINT_HH\n");
  program.push_back("#define CHECKPOINT_HH\n");
  program.push_back("\n");
  program.push_back("#include \"path.h\"\n");
  program.push_back("#include \"TraceInfo.h\"\n");
  program.push_back("#include \"ProgramState.hh\"\n");
  program.push_back("#include \"ProgramStates.hh\"\n");
  program.push_back("\n");
  program.push_back("#include <pthread.h>\n");
  program.push_back("#include <typeinfo>\n");
  program.push_back("#include <cassert>\n");
  program.push_back("\n");
  insertChkPtFuncs();
  program.push_back("\n");
  program.push_back("#endif\n");
}

ChkPtProgram::~ChkPtProgram() {
  fout.close();
}

// bool ChkPtProgram::equals(const std::set<std::string> &lhs, const std::set<std::string> &rhs) {
//   std::set<std::string> diff;
//   std::set_difference(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::inserter(diff, diff.begin()));
//   return (diff.empty()) ? true : false;
// }

void ChkPtProgram::insertChkPtFuncs() {
  std::set<std::set<std::string> > chkPtFuncArgs;
  for (auto &&args : vars)
    chkPtFuncArgs.insert(args);

  // for (auto &&args : chkPtFuncArgs)
  //   for (auto &&arg : args)
  //     std::cout << arg << std::endl;

  PSProgram psprogram;
  for (auto &&args : chkPtFuncArgs) {
    std::map<std::string, std::string> psVars;
    program.push_back("void checkpoint(");
    program.push_back("\n");
    auto insert_idx = program.size() - 1;

    std::string traceInfoVar, graphVar;
    bool insert_arg(true);
    for (auto &&arg : args) {
      if (arg == "traceInfo")
	continue;
      if (argList.find(arg) != argList.end()) {
	if (insert_arg && startswith(argList[arg], "Path")) {
	  std::string arg_insert("const ");
	  arg_insert += argList[arg];
	  if (psVars.find("p") == psVars.end())
	    psVars["p"] = argList[arg];
	  arg_insert += " ";
	  arg_insert += "&";
	  arg_insert += "p";
	  arg_insert += ", ";
	  arg_insert += "\n";
	  program[insert_idx] = arg_insert;
	  insert_arg = false;
	}
	// std::cout << argList[arg] << " " << arg << std::endl;
	if (!startswith(argList[arg], "const ")
	    && !endswith(argList[arg], "*")
	    && argList[arg] != "TraceInfo")
	  if (psVars.find(arg) == psVars.end())
	    psVars[arg] = argList[arg];
	if (argList[arg] == "TraceInfo")
	  traceInfoVar = arg;
	if (startswith(argList[arg], "Graph"))
	  graphVar = arg;
	std::string param(argList[arg]);
	param += " ";
	// if (startswith(argList[arg], "const "))
	if (!endswith(argList[arg], "*"))
	  param += "&";
	param += arg;
	param += ", ";
	param += "\n";
	program.push_back(param);
      }
      else {
	for (auto &&var : argList) {
	  std::string str(arg);
	  str += "[";
	  if (startswith(var.first, str)) {
	    // std::cout << var.first << " " << var.second << std::endl;
	    if (psVars.find(var.first) == psVars.end())
	      psVars[var.first] = var.second;
	    std::string param(var.second);
	    param += " ";
	    // if (startswith(argList[arg], "const "))
	    // param += "&";
	    param += var.first;
	    param += ", ";
	    param += "\n";
	    program.push_back(param);
	  }
	}
      }
    }
    program.push_back("const unsigned &label\n");
    program.push_back(")");
    program.push_back(" {");
    program.push_back("\n");
    program.push_back("switch (this_traceInfo.getJob()) {\n");

    // case RECORD
    program.push_back("case RECORD: {\n");
    program.push_back("switch (label) {\n");
    for (auto &&idx_vars_pair : cmpChkPtVars) {
      std::string case_stmt("case ");
      if (idx_vars_pair.second == args) {
    	case_stmt += itoa(idx_vars_pair.first);
    	case_stmt += ":\n";
    	// std::cout << idx_vars_pair.first << std::endl;
    	program.push_back(case_stmt);
      }
    }
    program.push_back("{\n");
    std::string psStmt(psprogram.addPSTy(psVars));
    program.push_back(psStmt);
    program.push_back(traceInfoVar);
    program.push_back(".setPSPtr(ps);\n");
    program.push_back(traceInfoVar);
    program.push_back(".setLabel(label);\n");
    program.push_back("if ((0 <= ");
    program.push_back(graphVar);
    program.push_back("->matrix[p.head()][p.rear()]");
    program.push_back(" && ");
    program.push_back(graphVar);
    program.push_back("->matrix[p.head()][p.rear()] < inf)");
    program.push_back(" || ");
    program.push_back("p.is_checked())\n");
    program.push_back("return;\n");
    program.push_back("ps->p.check();\n");
    program.push_back("p.check();\n");
    program.push_back("TraceInfo traceInfo(");
    program.push_back(traceInfoVar);
    program.push_back(");\n");
    program.push_back("traceInfo.setJob(RESUME);\n");
    program.push_back("pthread_mutex_lock(&");
    program.push_back(graphVar);
    program.push_back("->v_psLock[p.head()][p.rear()]);\n");
    program.push_back(graphVar);
    program.push_back("->v_tracePoints[p.head()][p.rear()].push_back(traceInfo);\n");
    program.push_back("pthread_mutex_unlock(&");
    program.push_back(graphVar);
    program.push_back("->v_psLock[p.head()][p.rear()]);\n");
    program.push_back("}\n");
    program.push_back("break;\n");
    program.push_back("default: {\n");
    program.push_back("if ((0 <= ");
    program.push_back(graphVar);
    program.push_back("->matrix[p.head()][p.rear()]");
    program.push_back(" && ");
    program.push_back(graphVar);
    program.push_back("->matrix[p.head()][p.rear()] < inf)");
    program.push_back(" || ");
    program.push_back("p.is_checked())\n");
    program.push_back("return;\n");
    program.push_back("p.check();\n");
    program.push_back(psStmt);
    program.push_back("ps->setGCFlag();\n");
    program.push_back("TraceInfo traceInfo(");
    program.push_back(traceInfoVar);
    program.push_back(");\n");
    program.push_back("traceInfo.setPSPtr(ps);\n");
    program.push_back("traceInfo.setLabel(label);\n");
    program.push_back("traceInfo.setJob(RESUME);\n");
    program.push_back("pthread_mutex_lock(&");
    program.push_back(graphVar);
    program.push_back("->v_psLock[p.head()][p.rear()]);\n");
    program.push_back(graphVar);
    program.push_back("->v_tracePoints[p.head()][p.rear()].push_back(traceInfo);\n");
    program.push_back("pthread_mutex_unlock(&");
    program.push_back(graphVar);
    program.push_back("->v_psLock[p.head()][p.rear()]);\n");
    program.push_back("}\n");
    program.push_back("break;\n");
    program.push_back("}\n");
    program.push_back("}\n");
    program.push_back("break;\n");
    program.push_back("\n");

    // case RESUME
    program.push_back("case RESUME: {\n");
    program.push_back(traceInfoVar);
    program.push_back(".setJob(RECORD);\n");
    std::string nullPSStm(psStmt.substr(0, psStmt.find("=") + 2));
    nullPSStm += "NULL;\n";
    program.push_back(nullPSStm);
    program.push_back("try {\n");
    program.push_back("ps = static_cast<");
    program.push_back(psStmt.substr(0, psStmt.find("=")));
    program.push_back("*>(");
    program.push_back(traceInfoVar);
    program.push_back(".getPSPtr());\n");
    program.push_back("} catch (const std::bad_cast &e) {\n");
    program.push_back("std::cout << e.what() << std::endl << __FILE__ << \": \" << __FUNCTION__ << \": \" << __LINE__ << std::endl;\n");
    program.push_back("}\n");
    program.push_back("\n");
    program.push_back("assert(NULL != ps && \"NULL Pointer!\\n\");\n");

    for (auto &&psVar : psVars) {
      std::cout << psVar.second << " " << psVar.first << std::endl;
      auto pos_lsqrBracket(psVar.first.find("["));
      if (pos_lsqrBracket == std::string::npos) {
	program.push_back(psVar.first);
	program.push_back(" = ");
	program.push_back("ps->");
	program.push_back(psVar.first);
	program.push_back(";\n");
      } else {
	auto arrayName(psVar.first.substr(0, pos_lsqrBracket));
	std::vector<std::string> maxSizes;
	std::vector<std::string> idx_strs;
	while (pos_lsqrBracket != std::string::npos) {
	  auto pos_rsqrBracket(matchSqrBracket(psVar.first, pos_lsqrBracket));
	  maxSizes.push_back(psVar.first.substr(pos_lsqrBracket + 1, pos_rsqrBracket - pos_lsqrBracket - 1));
	  pos_lsqrBracket = psVar.first.find("[", pos_rsqrBracket + 1);
	}
	for (std::size_t idx_maxSizes = 0; idx_maxSizes != maxSizes.size(); ++idx_maxSizes) {
	  program.push_back("for (std::size_t ");
	  std::string idx_str("idx");
	  idx_str += itoa(idx_maxSizes);
	  idx_strs.push_back(idx_str);
	  program.push_back(idx_str);
	  program.push_back(" = 0; ");
	  program.push_back(idx_str);
	  program.push_back(" != ");
	  program.push_back(maxSizes[idx_maxSizes]);
	  program.push_back("; ++");
	  program.push_back(idx_str);
	  program.push_back(") {\n");
	}
	program.push_back(arrayName);
	for (std::size_t idx = 0; idx != idx_strs.size(); ++idx) {
	  program.push_back("[");
	  program.push_back(idx_strs[idx]);
	  program.push_back("]");
	}
	program.push_back(" = ps->");
	program.push_back(arrayName);
	for (std::size_t idx = 0; idx != idx_strs.size(); ++idx) {
	  program.push_back("[");
	  program.push_back(idx_strs[idx]);
	  program.push_back("]");
	}
	program.push_back(";\n");
	for (std::size_t idx = 0; idx != idx_strs.size(); ++idx) {
	  program.push_back("}\n");
	}
      }
    }

    program.push_back("\n");
    program.push_back("ps = NULL;\n");
    program.push_back("}\n");
    program.push_back("break;\n");
    program.push_back("\n");
    program.push_back("default:\n");
    program.push_back("break;\n");
    program.push_back("}\n");
    program.push_back("}\n");
  }

  psprogram.writeIntoFile();
}

std::string ChkPtProgram::chkPtStmt(const std::string &stmt) {
  std::string ret(stmt);
  std::size_t pos_idx_start(strlen("checkpoint")),
    pos_idx_end(stmt.find("(")),
    pos_insert(stmt.find(")") - 1);
  std::string str_idx(stmt.substr(pos_idx_start, pos_idx_end - pos_idx_start));
  auto idx = atoi(str_idx.c_str());
  // ret += "checkpoint(";
  // ret += stmt.substr(pos_idx_end + 1, stmt.find(")") - pos_idx_end - 1);
  // ret += ");";
  // ret += "\n";
  ret.erase(pos_idx_start, pos_idx_end - pos_idx_start);

  std::string insert(", ");
  for (auto &&var : vars[idx]) {
    if (var == "traceInfo")
      continue;
    insert += var;
    insert += ", ";
  }
  insert += str_idx;
  ret.insert(pos_insert, insert);

  return ret;
}

void ChkPtProgram::writeIntoFile() {
  for (auto &&stmt : program)
    fout << stmt;
}
