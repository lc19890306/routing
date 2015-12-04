#include "Instrumenter.hh"

char Instrumenter::ID = 0;
static RegisterPass<Instrumenter> X("instrumenter", "Instrumenter Pass");

bool Instrumenter::runOnModule(Module &M) {
  // preprocess();
  for (Module::iterator i = M.begin(), e = M.end(); i != e; ++i) {
    Function *func = &(*i);
  // errs() << "Instrumenter: ";
  // errs().write_escaped(getFuncName(func->getName())) << '\n';

    if (!getFuncName(func->getName()).compare("Dijkstra"))
      runOnFunction(*func);
  }
  return true;
}

bool Instrumenter::runOnFunction(Function &F) {
  for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i) {
    BasicBlock *bb = &(*i);
    runOnBasicBlock(*bb);
  }

  fout << "#####\n";
  for (auto && var_pair : localVars) {
    for (auto && element : var_pair.second) {
      errs() << var_pair.first << " " << element << "\n";
      fout << var_pair.first << " " << element << "\n";
    }
  }
  // errs().write_escaped(getFuncName(StringRef("string::Dijkstra("))) << '\n';
  return true;
}

bool Instrumenter::runOnBasicBlock(BasicBlock &BB) {
  // BB.setName(itoa(BBCounter++));
  auto bbName = BB.getName();
  auto bbID = atoi(bbName.str().c_str());
  // errs() << "BB: " << bbID << "\n";
  // errs() << "predecessors: ";
  std::vector<unsigned> directPreds;
  unsigned predID(0);
  for (auto pi = pred_begin(&BB), pe = pred_end(&BB); pi != pe; ++pi) {
    BasicBlock *pred = *pi;
    auto predName = pred->getName();
    predID = atoi(predName.str().c_str());
    if (predID >= bbID)
      continue;
    // errs() << predID << " ";
    directPreds.push_back(predID);
    if (predecessors.size() <= bbID)
      predecessors.resize(bbID + 1);
    for (auto && element : predecessors[predID])
      predecessors[bbID].insert(element);
    predecessors[bbID].insert(predID);
  }
  if (directPreds.size() > 1) {
    predID = GCP(directPreds);
    // errs() << "\nGCP: " << predID;
  }
  // errs() << "\n";
  // errs() << "successors: ";
  // for (auto si = succ_begin(&BB), se = succ_end(&BB); si != se; ++si) {
  //   BasicBlock *succ = *si;
  //   auto succName = succ->getName();
  //   auto succID = atoi(succName.str().c_str());
  //   if (bbID >= succID)
  //     continue;
  //   errs() << succID << " ";
  // }
  // errs() << "\n";

  for (BasicBlock::iterator i = BB.begin(), e = BB.end(); i != e; ++i) {
    Instruction *inst = &(*i);
    // inst->dump();
    // if (CallInst *callI = dyn_cast<CallInst>(inst)) {
    // 	Function *func = callI->getCalledFunction();
    // 	StringRef funcName = getFuncName(func->getName());
    // 	errs() << funcName << "\n";
    // 	if (!funcName.compare("compare")) {
    // 	  errs() << funcName << "\n";
    // 	  callI->dump();
    // 	}
    // }

    // if (AllocaInst *allocaInst = dyn_cast<AllocaInst>(inst)) {
    //   StringRef varName = allocaInst->getName();
    //   if (!varName.empty()) {
    // 	allocaInst->getType()->dump();
    // 	errs() << " " << varName << "\n";
    // 	allocaInst->dump();
    //   }
    // }

    if (InvokeInst *invokeI = dyn_cast<InvokeInst>(inst)) {
      Function *func = invokeI->getCalledFunction();
      StringRef funcName = getFuncName(func->getName());
      if (!funcName.compare("compare")) {
	if (invokeI->getNumArgOperands() > 2) {
	  // errs() << funcName << "\n";
	  fout << "COMPARE" << "\n";
	// invokeI->dump();
	}
      }
      if (funcName.startswith("checkpoint")) {
    	// size_t numArgOperands = invokeI->getNumArgOperands();
    	// errs() << demangleName(funcName) << ": " << numArgOperands << "\n";
    	// for (size_t index = 0; index != numArgOperands; ++index) {
    	//   errs() << "Operand" << index << ": ";
    	//   invokeI->getArgOperand(index)->dump();
    	//   // errs() << "\n" << ;
    	// }
	std::size_t idx = atoi(funcName.substr(strlen("checkpoint")).str().c_str());
	if (resultVars.size() <= idx)
	  resultVars.resize(idx + 1);
	resultVars[idx] = currentVars[predID];
	for (auto && var_pair : localVars)
	  resultVars[idx].insert(var_pair.first);
    	// invokeI->dump();
    	errs() << "checkpoint" << idx << "\n";
	fout << "checkpoint\n" << idx << "\n";
	for (auto && element : resultVars[idx]) {
	    errs() << element << "\n";
	    fout << element << "\n";
	}
      }
    }

    if (DbgDeclareInst *dbgDeclareInst = dyn_cast<DbgDeclareInst>(inst)) {
      // dbgDeclareInst->dump();
      // Value *varType = dbgDeclareInst->getVariable()->getOperand()
      // for (size_t i = 0, e = dbgDeclareInst->getVariable()->getNumOperands(); i != e; ++i)
    	// dbgDeclareInst->getVariable()->getOperand(i)->dump();
      // dbgDeclareInst->getVariable()->getOperand(3)->dump();
      Value *var = dbgDeclareInst->getVariable()->getOperand(2);
      Type *type = dbgDeclareInst->getAddress()->getType();
      // type->dump();
      // errs() << type->getPointerElementType()->getTypeID() << ":";
      // if (type->isPointerTy())
      std::string varName = var->getName().str();
      // ptrFlag = false;
      std::string typeName = getTypeName(type).str();
      // errs() << typeName;
      // errs() << ":" << varName << "\n";
      if (localVars.find(varName) == localVars.end()) {
	if (localVars[varName].find(typeName) == localVars[varName].end())
	  localVars[varName].insert(typeName);
	std::size_t pos = typeName.find("int");
	std::string tmpTypeName;
	if (pos != std::string::npos) {
	  tmpTypeName = typeName;
	  tmpTypeName.replace(pos, strlen("int"), "unsigned");
	  localVars[varName].insert(tmpTypeName);
	  tmpTypeName = typeName;
	  tmpTypeName.replace(pos, strlen("int"), "unsigned int");
	  localVars[varName].insert(tmpTypeName);
	  continue;
	}
	pos = typeName.find("short");
	if (pos != std::string::npos) {
	  tmpTypeName = typeName;
	  tmpTypeName.replace(pos, strlen("short"), "unsigned short");
	  localVars[varName].insert(tmpTypeName);
	  continue;
	}
	pos = typeName.find("long");
	if (pos != std::string::npos) {
	  tmpTypeName = typeName;
	  tmpTypeName.replace(pos, strlen("long"), "unsigned long");
	  localVars[varName].insert(tmpTypeName);
	  continue;
	}
      }
    }
  }

  if (currentVars.size() <= bbID)
    currentVars.resize(bbID + 1);
  currentVars[bbID] = currentVars[predID];
  for (auto && var_pair : localVars)
    currentVars[bbID].insert(var_pair.first);
  return true;
}
    
StringRef Instrumenter::demangleName(const StringRef mangledName) {
  int status(0);
  const char *result = abi::__cxa_demangle(mangledName.data(), 0, 0, &status);
  if (result == NULL)
    return mangledName;
  return StringRef(result);
}

StringRef Instrumenter::strip(const StringRef demangledName) {
  // errs() << demangledName << "\tsize: " << demangledName.size() << "\n";
  size_t end = demangledName.find('(');
  // errs() << demangledName << "\tend: " << end << "\n";
  size_t begin = demangledName.find_last_of("::", end) + 1;
  // errs() << demangledName << "\tbegin: " << begin << "\n";
  return demangledName.substr(begin, end - begin);
}

StringRef Instrumenter::getFuncName(const StringRef mangledName) {
  return strip(demangleName(mangledName));
}

StringRef Instrumenter::getTypeName(const Type *type) {
  std::string ret;
  switch (type->getTypeID()) {
  case Type::VoidTyID:
    ret += "void";
    break;
  case Type::FloatTyID:
    ret += "float";
    break;
  case Type::DoubleTyID:
    ret += "double";
    break;
  case Type::IntegerTyID: {
    if (type->isIntegerTy(8))
      ret += "bool";
    if (type->isIntegerTy(16))
      ret += "short";
    if (type->isIntegerTy(32))
      ret += "int";
    if (type->isIntegerTy(64))
      ret += "long";
  }
    break;
  case Type::StructTyID:
    ret += type->getStructName().str().substr(strlen("class."));
    break;
  case Type::ArrayTyID: {
    ret += getTypeName(type->getArrayElementType()).str();
    ret += "[";
    ret += itoa(type->getArrayNumElements());
    ret += "]";
  }
    break;
  case Type::PointerTyID: {
    ret += getTypeName(type->getPointerElementType()).str();
    // if (ptrFlag)
    //   ret += "*";
    // ptrFlag = true;
  }
    break;
  case Type::VectorTyID: {
    ret += "std::vector<";
    ret += getTypeName(type->getVectorElementType()).str();
    ret += " >";
  }
    break;
  default:
    break;
    // return StringRef();
  }

  return StringRef(ret);
}

unsigned Instrumenter::GCP(const std::vector<unsigned> &directPreds) {
  unsigned ret(0);
  std::set<unsigned> intersect(predecessors[directPreds[0]]);
  for (std::size_t idx = 1; idx != directPreds.size(); ++idx) {
      std::set_intersection(intersect.begin(),
			    intersect.end(),
			    predecessors[directPreds[idx]].begin(),
			    predecessors[directPreds[idx]].end(),
			    std::inserter(intersect, intersect.begin()));
    }
  if (!intersect.empty())
    ret = *intersect.rbegin();

  return ret;
}
