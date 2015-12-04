#ifndef __H_TRACE_INFO_QUEUE_
#define __H_TRACE_INFO_QUEUE_

#include "concurrentqueue.h"

/* #include <deque> */
#include <pthread.h>

#include "TraceInfo.h"

class TraceInfoQueue {
 public:
  /* static std::queue<TraceInfo> _queue; */
  /* static pthread_mutex_t _lock; */
  static std::vector<moodycamel::ConcurrentQueue<TraceInfo> > v_queue;
  /* static std::vector<std::deque<TraceInfo> > v_queue; */
  /* static std::vector<pthread_mutex_t> v_lock; */
};

#endif	/* __H_TRACE_INFO_QUEUE_ */
