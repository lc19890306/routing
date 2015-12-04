#ifndef __HH_ALGOJOB_
#define __HH_ALGOJOB_

#include <ck_pr.h>

#include "headers.h"
#include "TThreadPool.hh"
#include "TraceInfo.h"
#include "Graph.hh"
#include "types.hh"
#include "dijkstra.h"

class AlgoJob : public ThreadPool::TPool::TJob {
public:
  AlgoJob(pAlgoFunc algoFunc, const TraceInfo &traceInfo, Graph *graph, const int &i = -1);

  ~AlgoJob();

  virtual void run(void *);

protected:
  pAlgoFunc _algoFunc;
  TraceInfo _traceInfo;
  Graph *_graph;
};

#endif	// __HH_ALGOJOB_
