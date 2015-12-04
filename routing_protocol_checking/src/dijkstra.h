#ifndef __H_DIJKSTRA_
#define __H_DIJKSTRA_

#include <ck_pr.h>
#include <pthread.h>

#include "headers.h"
#include "path.h"
#include "TraceInfo.h"
#include "Graph.hh"
#include "compare.h"
#include "functions.h"
#include "traceVector.h"
#include "propertycheck.hh"

/* typedef struct { */
  /* int n; */
  /* TraceInfo traceInfo; */
  /* int v; */
  /* vector<TraceInfo> *traces; */
  /* int c[maxnum][maxnum]; */
  /* CBStringList possible_paths; */
  /* pthread_cond_t *cond; */
  /* pthread_mutex_t *lock; */
/* } Params; */

void initialize(std::vector<std::vector<Path<int> > > &path);
/* void initialize(Path<int> path[maxnum][maxnum], int n = 0); */
void Dijkstra(const TraceInfo &traceInfo, Graph *graph);
/* void *thread_routine(void *arg); */
/* Params createParams(const int &n, const int &v, const int c[maxnum][maxnum], const TraceInfo &traceInfo, const vector<string> &possible_paths); */
/* Params *createParams(const TraceInfo &traceInfo); */

#endif	/* __H_DIJKSTRA_ */
