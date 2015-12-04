#include "preprocessor.hh"

bool cmpFuncExists(const std::string &stmt, const std::size_t &pos) {
  std::size_t cmp_pos = stmt.find("compare");
  if (cmp_pos != std::string::npos && pos == std::string::npos) {
    if (!isalnum(stmt[cmp_pos - 1]) && stmt[cmp_pos - 1] != '_') {
      std::size_t pos_lparenthesis(stmt.find('(', cmp_pos + strlen("compare")));
      if (pos_lparenthesis == std::string::npos)
	return false;
      for (std::size_t i = cmp_pos + strlen("compare"), e = pos_lparenthesis; i != e; ++i)
	if (stmt[i] != ' ' || stmt[i] != '\t')
	  return false;
      std::stack<std::size_t> stk_parenthesis;
      stk_parenthesis.push(pos_lparenthesis);
      std::size_t i(pos_lparenthesis + 1);
      unsigned counter(0);
      while (!stk_parenthesis.empty()) {
	switch (stmt[i]) {
	case '(':
	  stk_parenthesis.push(i);
	  break;
	case ')':
	  stk_parenthesis.pop();
	  break;
	case ',':
	  ++counter;
	  break;
	default:
	  break;
	}
      }
      if (counter >= 3)
	return true;
    }
  }
  return false;
}

std::size_t matchSqrBracket(const std::string &stmt, const std::size_t &pos) {
  std::size_t ret(0);
  std::stack<std::size_t> stk_sqrBracket;
  stk_sqrBracket.push(pos);
  std::size_t i(pos + 1);
  while (!stk_sqrBracket.empty()) {
    switch (stmt[i]) {
    case '[':
      stk_sqrBracket.push(i);
      break;
    case ']': {
      stk_sqrBracket.pop();
      ret = i;
    }
      break;
    default:
      break;
    }
    ++i;
  }
  return ret;
}

int getArgVar(const std::string &stmt, const std::string &varName, const std::size_t &pos, std::string &path) {
  std::size_t pos_lsqrBracket1(stmt.find("[", pos + varName.size() - 1));
  if (pos_lsqrBracket1 == std::string::npos)
    return -1;
  std::string interm(stmt.substr(pos + varName.size(), pos_lsqrBracket1 - pos - varName.size()));
  strip(interm, " \t");
  if (!interm.empty())
    return -1;
  std::size_t pos_rsqrBracket1(matchSqrBracket(stmt, pos_lsqrBracket1));

  std::size_t pos_lsqrBracket2(stmt.find("[", pos_rsqrBracket1 + 1));
  if (pos_lsqrBracket2 == std::string::npos)
    return -1;
  interm = stmt.substr(pos_rsqrBracket1 + 1, pos_lsqrBracket2 - pos_rsqrBracket1 - 1);
  strip(interm, " \t");
  if (!interm.empty())
    return -1;
  std::size_t pos_rsqrBracket2(matchSqrBracket(stmt, pos_lsqrBracket2));
  path = stmt.substr(pos, pos_rsqrBracket2 - pos + 1);
  // std::cout << path << std::endl;
  return 0;
}

void preprocess(const std::string &filename, const std::string &funcName, const std::string &varName) {
  PreProcProgram program(filename);
  std::string stmt;
  bool func_found = false;
  unsigned long chkpt_counter(0);
  program.push_back("#include \"chkpt.hh\"\n");
  while (program.getline(stmt)) {
    if (!func_found && stmt.find(funcName) != std::string::npos)
      func_found = true;
    if (!func_found) {
      stmt += "\n";
      program.push_back(stmt);
      continue;
    }
    if (!stmt.empty())
      strip(stmt, " \t");
    if (startswith(stmt, "//"))
      continue;
    std::size_t pos = stmt.find(varName);
    if (cmpFuncExists(stmt, pos)) {
      std::string chkpt_stmt("L");
      chkpt_stmt += itoa(chkpt_counter);
      chkpt_stmt += ":\ncheckpoint";
      chkpt_stmt += itoa(chkpt_counter++);
      chkpt_stmt += "(";
      chkpt_stmt += "path[0][0]";
      chkpt_stmt += ");\n";
      program.push_back(chkpt_stmt);
    }
    if (stmt.find("Path") == std::string::npos) {
      while (pos != std::string::npos) {
	if (!isalnum(stmt[pos - 1]) && stmt[pos - 1] != '_') {
	  std::string var;
	  if (!getArgVar(stmt, varName, pos, var)) {
	    std::cout << var << std::endl << stmt << std::endl;
	    std::string chkpt_stmt("L");
	    chkpt_stmt += itoa(chkpt_counter);
	    chkpt_stmt += ":\ncheckpoint";
	    chkpt_stmt += itoa(chkpt_counter++);
	    chkpt_stmt += "(";
	    chkpt_stmt += var;
	    chkpt_stmt += ");\n";
	    program.push_back(chkpt_stmt);
	  }
	}
	pos = stmt.find(varName, pos + varName.size());
      }
    }
    else {
      if (pos != std::string::npos) {
	unsigned i(pos - 1);
	while (stmt[i] == ' ' || stmt[i] == '\t') {
	  ++i;
	}
	if (stmt[i] == ',' || stmt[i] == '=')
	  while (pos != std::string::npos) {
	    if (!isalnum(stmt[pos - 1]) && stmt[pos - 1] != '_') {
	      std::string var;
	      if (!getArgVar(stmt, varName, pos, var)) {
		std::cout << var << std::endl << stmt << std::endl;
		std::string chkpt_stmt("L");
		chkpt_stmt += itoa(chkpt_counter);
		chkpt_stmt += ":\ncheckpoint";
		chkpt_stmt += itoa(chkpt_counter++);
		chkpt_stmt += "(";
		chkpt_stmt += var;
		chkpt_stmt += ");\n";
		program.push_back(chkpt_stmt);
	      }
	    }
	    pos = stmt.find(varName, pos + varName.size());
	  }
      }
    }
    stmt += "\n";
    program.push_back(stmt);
  }
  ChkPtProgram chkPtProgram(chkpt_counter);
  program.writeIntoFile();
  chkPtProgram.writeIntoFile();
}
