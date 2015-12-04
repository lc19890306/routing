#ifndef __H_COMPARE_
#define __H_COMPARE_

#include <pthread.h>

#include "headers.h"
/* #include "dijkstra.h" */
#include "bellman_ford.hh"
#include "AlgoJob.hh"
#include "path.h"
#include "relational_db.h"
#include "TraceInfo.h"

int compare(const int &a, const int &b);
int compare(const Path<int> &p1, const Path<int> &p2, TraceInfo &traceInfo, Graph *graph);

#if 0
int compare(vector<int> path1, vector<int> path2) {
  int res = 0;
  string p1 = "", p2 = "";
  int i = 0;
  int size1 = path1.size();
  int size2 = path2.size();
  if (0 != size1)
    for (i = 0; i < size1; ++i) {
      char c = path1[i] + '0';
      p1 += c;
    }
  if (0 != size2)
    for (i = 0; i < size2; ++i) {
      char c = path2[i] + '0';
      p2 += c;
    }
  /* res = relation[p1][p2]; */
  return res;
}
#endif

#endif	/* __H_COMPARE_ */
