#ifndef __HH_GRAPH_
#define __HH_GRAPH_

#include <vector>
#include <string>
#include <bitset>
#include <algorithm>
#include <cassert>
#include <deque>

// #include "tbb/concurrent_vector.h"

#include "headers.h"
#include "path.h"
#include "functions.h"
#include "TraceInfo.h"
// #include "ProgramState.hh"
// #include "TraceInfoTree.hh"

typedef struct {
  String path;
  std::bitset<maxnum> nodes;
} StrPath;

class Graph {
public:
  Graph();
  Graph(const unsigned &num);
  Graph(const Graph &graph);
  // Graph & operator = (const Graph &graph);
  Graph(Graph &&graph);
  Graph & operator = (Graph graph);
  // Graph & operator = (Graph &&graph);
  virtual ~Graph() noexcept;

  unsigned getNum() const;
  unsigned weight(const Path<int> &path);
  unsigned weight(Path<int> &&path);
  unsigned weight(std::vector<int> path[maxnum][maxnum], unsigned s, unsigned t1, unsigned t2 = 0);
  void deriveGraph(const Graph &graph, unsigned src, unsigned tgt);
  void deriveGraph(Graph &&baseGraph, unsigned src, unsigned tgt);
  unsigned getMaxLen() const;
  double getSum() const;
  void setSum(const double &sum);
  unsigned getLastSrc() const;
  void setLastSrc(const unsigned &src);
  unsigned getLastTgt() const;
  void setLastTgt(const unsigned &tgt);
  unsigned getNewEdgesCounter() const;
  void setNewEdgesCounter(const unsigned &newEdgesCounter);
  unsigned getFlagTgt() const;
  void setFlagTgt(const unsigned &tgt);
  void printMatrix() const;
  void clearInitTraces();
  unsigned getNewSrc() const;
  unsigned getNewTgt() const;
  void collectGarbage();

  std::vector<std::vector<int> > matrix;
  std::vector<std::vector<std::vector<StrPath> > > v_possible_paths;
  std::vector<std::vector<TraceInfo> > v_traces;
  // std::vector<std::vector<TraceInfo> > v_initTraces;
  std::vector<pthread_mutex_t> v_lock;
  std::vector<std::vector<std::deque<TraceInfo> > > v_tracePoints;
  // tbb::concurrent_vector<tbb::concurrent_vector<std::deque<TraceInfo> > > v_tracePoints;
  std::vector<std::vector<pthread_mutex_t> > v_psLock;

private:
  void deriveAllPossiblePaths(const Graph &graph, const unsigned &src, const unsigned &tgt);
  void clear();
  void swap(Graph &other);

  unsigned _num;
  unsigned maxLen;
  double sum;
  unsigned lastSrc;
  unsigned lastTgt;
  unsigned newEdgesCounter;
  unsigned flagTgt;
  unsigned newSrc;
  unsigned newTgt;
};

#endif	// __HH_GRAPH_
