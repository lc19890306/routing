#ifndef INSTRUMENTER_HH
#define INSTRUMENTER_HH

#define DEBUG_TYPE "instrumenter"

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Metadata.h"
#include "string.hh"
// #include "preprocessor.hh"

#include <cxxabi.h>
#include <map>
#include <set>
#include <fstream>

using namespace llvm;

STATISTIC(BBCounter, "Counts number of basic blocks greeted");

namespace {
  struct Instrumenter : public ModulePass {
  public:
    static char ID; // Pass identification, replacement for typeid
    // bool ptrFlag;
    std::map<std::string, std::set<std::string> > localVars;
    std::vector<std::set<std::string> > currentVars;
    std::vector<std::set<std::string> > resultVars;
    std::vector<std::set<unsigned> > predecessors;
    std::ofstream fout;
    Instrumenter() : ModulePass(ID) {
      fout.open("vars.txt", std::ios_base::out);
    }
    ~Instrumenter() {
      fout.close();
    }
    
    virtual bool runOnModule(Module &M);
    virtual bool runOnFunction(Function &F);
    virtual bool runOnBasicBlock(BasicBlock &BB);
    StringRef demangleName(const StringRef mangledName);
    StringRef strip(const StringRef demangledName);
    StringRef getFuncName(const StringRef mangledName);
    StringRef getTypeName(const Type *type);
    unsigned GCP(const std::vector<unsigned> &directPreds);
  };
}

#endif	// INSTRUMENTER_HH
