#ifndef PROPERTYCHECK_HH
#define PROPERTYCHECK_HH

#include "dijkstra.h"
#include "types.hh"
#include "CheckConsistencyJob.hh"

#include <mutex>

void checkOptimality(Graph *graph, TraceInfo &this_traceInfo, const Path<int> &path);
void checkConsistency(const pAlgoFunc &algoFunc, const Graph &graph);//, const std::size_t &v, const std::size_t &n);
void checkConsistency(const pAlgoFunc &algoFunc, const Graph &graph, const TraceInfo &traceInfo, const std::size_t &i);

#endif	// PROPERTYCHECK_HH
