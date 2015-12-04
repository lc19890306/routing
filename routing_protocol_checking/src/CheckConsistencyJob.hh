#ifndef CHECKCONSISTENCYJOB_HH
#define CHECKCONSISTENCYJOB_HH

#include <ck_pr.h>

#include "headers.h"
#include "TThreadPool.hh"
#include "TraceInfo.h"
#include "Graph.hh"
#include "types.hh"
#include "dijkstra.h"
#include "propertycheck.hh"

class CheckConsistencyJob : public ThreadPool::TPool::TJob {
public:
  CheckConsistencyJob(const pAlgoFunc &algoFunc, const Graph &graph, const TraceInfo &traceInfo, const std::size_t &idx, const int &i = -1);
  ~CheckConsistencyJob();
  virtual void run(void *);

protected:
  pAlgoFunc _algoFunc;
  Graph _graph;
  TraceInfo _traceInfo;
  std::size_t _idx;
};

#endif	// CHECKCONSISTENCYJOB_HH
