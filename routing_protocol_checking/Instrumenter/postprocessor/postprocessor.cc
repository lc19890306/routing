#include "postprocessor.hh"

void postprocess(const std::string &filename, const std::string &funcName) {
  PostProcProgram program(filename);
  std::vector<std::set<std::string> > vars;
  std::map<std::string, std::set<std::string> > varList;
  std::map<std::string, std::string> argList;
  std::map<std::size_t, std::set<std::string> > cmpChkPtVars;

  std::ifstream fin;
  fin.open("vars.txt", std::ios_base::in);
  std::string line, varName, typeName;
  std::size_t idx(0);
  bool finSwitch = false;
  while (getline(fin, line)) {
    if (line.empty())
      continue;
    if (line == "checkpoint") {
      fin >> idx;
      if (vars.size() <= idx)
	vars.resize(idx + 1);
      fin.sync();
      continue;
    }
    if (line == "COMPARE") {
      if (cmpChkPtVars.find(idx) == cmpChkPtVars.end())
	cmpChkPtVars[idx] = vars[idx];
      continue;
    }
    if (line == "#####") {
      finSwitch = true;
      continue;
    }
    if (finSwitch) {
      std::size_t pos_delim_start = line.find(' ');
      varName = line.substr(0, pos_delim_start);
      std::size_t pos_delim_end = line.find_first_not_of(' ', pos_delim_start);
      typeName = line.substr(pos_delim_end, line.size() - pos_delim_end + 1);
      // fin >> varName >> typeName;
      // if (varList.find(varName) == varList.end())
      if (varList[varName].find(typeName) == varList[varName].end())
	varList[varName].insert(typeName);
      // else
    }
    else {
      // if (vars[idx].find(varName) == vars[idx].end())
      // fin >> varName;
      // if (vars[idx][varName].find(typeName) == vars[idx][varName].end())
      vars[idx].insert(line);
    }
  }
  fin.close();

  program.push_back("#include \"checkpoint.hh\"\n");
  std::vector<std::size_t> ckpt_linum;
  std::string stmt;
  bool func_found(false);
  while (program.getline(stmt)) {
    if (stmt.find("chkpt.hh") != std::string::npos)
      continue;
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
    auto ckpt_pos = stmt.find("checkpoint");
    if (ckpt_pos != std::string::npos) {
      auto idx_start_pos = ckpt_pos + strlen("checkpoint");
      auto idx_end_pos = stmt.find("(", idx_start_pos);
      idx = atoi(stmt.substr(idx_start_pos, idx_end_pos - idx_start_pos).c_str());
      if (ckpt_linum.size() <= idx)
	ckpt_linum.resize(idx + 1);
      stmt += "\n";
      ckpt_linum[idx] = program.size();
      program.push_back(stmt);
      continue;
    }
    for (auto varListIter = varList.begin(); varListIter != varList.end(); ++varListIter) {
      auto pos_lfirst = find(stmt, varListIter->first);//stmt.find(varListIter->first);
      // auto pos_rfirst = pos_lfirst + varListIter->first.size();
      if (pos_lfirst == std::string::npos)
	continue;
      for (auto typeIter = varListIter->second.rbegin(); typeIter != varListIter->second.rend(); ++typeIter) {
	std::size_t pos_type;
	// if (typeIter->find("<") != std::string::npos) {
	//   if (argList.find(varListIter->first) == argList.end())
	//     argList[varListIter->first] = *typeIter;
	//   continue;
	// }

	// Array type
	pos_type = typeIter->find("[");
	if (pos_type != std::string::npos) {
	  typeName = typeIter->substr(0, pos_type);
	  strip(typeName, " \t");
	  std::size_t pos_type_stmt(find(stmt, typeName));
	  if (pos_type_stmt == std::string::npos)
	    continue;
	  std::size_t pos_langleBracket(stmt.find("<", pos_type_stmt + typeName.size()));
	  if (pos_langleBracket != std::string::npos) {
	    std::string empty(stmt.substr(pos_type_stmt + typeName.size(), pos_langleBracket - pos_type_stmt - typeName.size()));
	    strip(empty, " \t");
	    assert(empty.empty() && "Error: faulty characters appear!");
	    std::size_t pos_rangleBracket(matchAngleBracket(stmt, pos_langleBracket));
	    typeName += stmt.substr(pos_langleBracket, pos_rangleBracket - pos_langleBracket + 1);
	  }
	  // Pointer type??
	  std::size_t ptr_end(stmt.find_last_not_of(" \t", pos_lfirst));
	  if (ptr_end != std::string::npos && stmt[ptr_end] == '*') {
	    std::size_t ptr_start(stmt.find_last_not_of("*", ptr_end) + 1);
	    typeName += stmt.substr(ptr_start, ptr_end - ptr_start + 1);
	  }
	  std::string dimensions(typeIter->substr(pos_type, typeIter->size() - pos_type));
	  strip(dimensions, " \t");
	  if (argList.find(varListIter->first + dimensions) == argList.end()) {
	    if (rfind(stmt, "const", find(stmt, typeName) - 1) != std::string::npos)
	      argList[varListIter->first + dimensions] += "const ";
	    argList[varListIter->first + dimensions] += typeName;
	  }
	  continue;
	}

	// Regular type
	typeName = *typeIter;
	pos_type = find(stmt, typeName);
	if (pos_type != std::string::npos) {
	  // std::cout << varListIter->first << std::endl;
	  // std::cout << stmt << std::endl;
	  std::size_t pos_langleBracket(stmt.find("<", pos_type + typeName.size()));
	  if (pos_langleBracket != std::string::npos) {
	    std::string empty(stmt.substr(pos_type + typeName.size(), pos_langleBracket - pos_type - typeName.size()));
	    strip(empty, " \t");
	    assert(empty.empty() && "Error: faulty characters appear!");
	    std::size_t pos_rangleBracket(matchAngleBracket(stmt, pos_langleBracket));
	    typeName += stmt.substr(pos_langleBracket, pos_rangleBracket - pos_langleBracket + 1);
	  }
	  // Pointer type??
	  std::size_t ptr_end(stmt.find_last_not_of(" \t", pos_lfirst));
	  if (ptr_end != std::string::npos && stmt[ptr_end] == '*') {
	    std::size_t ptr_start(stmt.find_last_not_of("*", ptr_end) + 1);
	    typeName += stmt.substr(ptr_start, ptr_end - ptr_start + 1);
	  }
	  if (argList.find(varListIter->first) == argList.end()) {
	    if (rfind(stmt, "const", pos_type - 1) != std::string::npos)
	      argList[varListIter->first] += "const ";
	    argList[varListIter->first] += typeName;
	  }
	}
      }
    }
    stmt += "\n";
    program.push_back(stmt);
  }

  // for (auto iter = argList.begin(); iter != argList.end(); ++iter)
  //   std::cout << iter->second << " " << iter->first << std::endl;

  ChkPtProgram chkPtProgram(vars, argList, cmpChkPtVars);
  for (auto &&linum : ckpt_linum) {
    program.replace(linum, chkPtProgram.chkPtStmt(program.at(linum)));
  }
  chkPtProgram.writeIntoFile();

  program.writeIntoFile();
}
