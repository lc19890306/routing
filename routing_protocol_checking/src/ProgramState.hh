#ifndef __HH_PROGRAM_STATE_
#define __HH_PROGRAM_STATE_

#include "path.h"
#include "TraceInfo.h"

#include <cstring>
#include <memory>

class ProgramState {
public:
  // ProgramState() : label(0), gcFlag(false) {}
  ProgramState(const unsigned &label = 0) : label(label), gcFlag(false) {}

  virtual ~ProgramState() {}

  unsigned getLabel() const {
    return label;
  }

  void setLabel(const unsigned &label) {
    this->label = label;
  }

  bool is_GCAllowed() const {
    return gcFlag;
  }

  void setGCFlag(const bool &gcFlag = true) {
    this->gcFlag = gcFlag;
  }

private:
  unsigned label;
  bool gcFlag;
};

// typedef ProgramState* pProgramState;
typedef std::shared_ptr<ProgramState> pProgramState;

typedef enum {RECORD, RESUME} CheckJob;

#endif	// __HH_PROGRAM_STATE_
