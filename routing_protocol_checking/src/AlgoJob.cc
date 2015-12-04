#include "AlgoJob.hh"

AlgoJob::AlgoJob(pAlgoFunc algoFunc, const TraceInfo &traceInfo, Graph *graph, const int &i) : ThreadPool::TPool::TJob(i), _algoFunc(algoFunc), _traceInfo(traceInfo), _graph(graph) {}

AlgoJob::~AlgoJob() {
  TraceInfo traceInfo;
  std::swap(traceInfo, _traceInfo);
  if (_graph != nullptr)
    _graph = nullptr;
}

void AlgoJob::run(void *) {
  ck_pr_inc_64((uint64_t *)&runningThread::counter);
  _algoFunc(_traceInfo, _graph);  
  ck_pr_dec_64((uint64_t *)&runningThread::counter);
  _graph = nullptr;
}
