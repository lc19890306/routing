#ifndef __H_TRACEVECTOR_
#define __H_TRACEVECTOR_

#include <vector>

#include "TraceInfo.h"

class Traces {
 public:
  static std::vector<std::vector<TraceInfo> > _vector;
  static std::vector<pthread_mutex_t> v_lock;
};

#endif	/* __H_TRACEVECTOR_ */
