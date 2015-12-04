#include "CheckConsistencyJob.hh"

CheckConsistencyJob::CheckConsistencyJob(const pAlgoFunc &algoFunc, const Graph &graph, const TraceInfo &traceInfo, const std::size_t &idx, const int &i) : ThreadPool::TPool::TJob(i), _algoFunc(algoFunc), _graph(graph), _traceInfo(traceInfo), _idx(idx) {}

CheckConsistencyJob::~CheckConsistencyJob() {
  Graph graph;
  std::swap(graph, _graph);
  TraceInfo traceInfo;
  std::swap(traceInfo, _traceInfo);
  // _graph.collectGarbage();
}

void CheckConsistencyJob::run(void *) {
  ++runningThread::counter;
  checkConsistency(_algoFunc, _graph, _traceInfo, _idx);
  --runningThread::counter;
}
