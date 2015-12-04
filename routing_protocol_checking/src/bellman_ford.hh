#ifndef BELLMAN_FORD_HH
#define BELLMAN_FORD_HH

#include "headers.h"
#include "path.h"
#include "TraceInfo.h"
#include "Graph.hh"
#include "compare.h"
#include "functions.h"
#include "traceVector.h"
#include "propertycheck.hh"

#include <ck_pr.h>
#include <pthread.h>

void initialize(std::vector<std::vector<Path<int> > > &path);
void Bellman_Ford(const TraceInfo &traceInfo, Graph *graph);

#endif	// BELLMAN_FORD_HH
