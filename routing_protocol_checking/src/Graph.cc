#include "Graph.hh"

Graph::Graph() : 
  _num(0), 
  maxLen(0),
  sum(0),
  lastSrc(0),
  lastTgt(0),
  newEdgesCounter(0),
  flagTgt(0),
  newSrc(0),
  newTgt(0) {}

Graph::Graph(const unsigned &num) : 
  matrix(num + 1, vector<int>(num + 1, inf)),
  v_possible_paths(std::vector<std::vector<std::vector<StrPath> > >(num + 1, std::vector<std::vector<StrPath> >(num + 1))), 
  v_traces(std::vector<std::vector<TraceInfo> >(num + 1)),
  v_lock(std::vector<pthread_mutex_t>(num + 1, PTHREAD_MUTEX_INITIALIZER)),
  v_tracePoints(std::vector<std::vector<std::deque<TraceInfo> > >(num + 1, std::vector<std::deque<TraceInfo> >(num + 1, std::deque<TraceInfo>()))),
  // v_tracePoints(tbb::concurrent_vector<tbb::concurrent_vector<std::deque<TraceInfo> > >(num + 1, tbb::concurrent_vector<std::deque<TraceInfo> >(num + 1, std::deque<TraceInfo>()))),
  v_psLock(std::vector<std::vector<pthread_mutex_t> >(num + 1, std::vector<pthread_mutex_t>(num + 1, PTHREAD_MUTEX_INITIALIZER))),
  _num(num),
  maxLen(1),
  sum(0),
  lastSrc(0),
  lastTgt(0),
  newEdgesCounter(0),
  flagTgt(0),
  newSrc(0),
  newTgt(0) {}

Graph::Graph(const Graph &graph) :
  matrix(graph.matrix),
  v_possible_paths(graph.v_possible_paths),
  v_traces(graph.v_traces),
  v_lock(graph.v_lock),
  v_tracePoints(graph.v_tracePoints),
  v_psLock(graph.v_psLock),
  _num(graph._num),
  maxLen(graph.maxLen),
  sum(graph.sum),
  lastSrc(graph.lastSrc),
  lastTgt(graph.lastTgt),
  newEdgesCounter(graph.newEdgesCounter),
  flagTgt(graph.flagTgt),
  newSrc(graph.newSrc),
  newTgt(graph.newTgt) {
  // for (std::size_t i = 0; i < graph.v_tracePoints.size(); ++i)
  //   for (std::size_t j = 0; j < graph.v_tracePoints[i].size(); ++j)
  //     for (std::size_t k = 0; k < graph.v_tracePoints[i][j].size(); ++k)
  // 	v_tracePoints[i][j].push_back(graph.v_tracePoints[i][j][k]);
}

Graph & Graph::operator = (Graph graph) {
  swap(graph);

  return *this;
}

Graph::Graph(Graph &&graph) :
  matrix(graph.matrix),
  v_possible_paths(graph.v_possible_paths),
  v_traces(graph.v_traces),
  v_lock(graph.v_lock),
  v_tracePoints(graph.v_tracePoints),
  v_psLock(graph.v_psLock),
  _num(graph._num),
  maxLen(graph.maxLen),
  sum(graph.sum),
  lastSrc(graph.lastSrc),
  lastTgt(graph.lastTgt),
  newEdgesCounter(graph.newEdgesCounter),
  flagTgt(graph.flagTgt),
  newSrc(graph.newSrc),
  newTgt(graph.newTgt) {
  // std::vector<std::vector<int> >().swap(graph.matrix);
  // std::vector<std::vector<std::vector<StrPath> > >().swap(graph.v_possible_paths);
  // std::vector<std::vector<TraceInfo> >().swap(graph.v_traces);
  // std::vector<pthread_mutex_t>().swap(graph.v_lock);
  // std::vector<std::vector<std::deque<TraceInfo> > >().swap(graph.v_tracePoints);
  // std::vector<std::vector<pthread_mutex_t> >().swap(graph.v_psLock);
  // for (std::size_t i = 0; i < graph.v_tracePoints.size(); ++i)
  //   for (std::size_t j = 0; j < graph.v_tracePoints[i].size(); ++j)
  //     for (std::size_t k = 0; k < graph.v_tracePoints[i][j].size(); ++k)
  // 	v_tracePoints[i][j].push_back(graph.v_tracePoints[i][j][k]);
}

Graph::~Graph() noexcept {
  clear();
  std::vector<std::vector<int> >().swap(matrix);
}

unsigned Graph::getNum() const {
  return _num;
}

unsigned Graph::weight(const Path<int> &path) {
  unsigned s(path.head());
  unsigned t(path.rear());

  int w = 0;
  int i = 0;
  int size = 0;

  if (path.empty())
    w = matrix[s][t];
  else {
    size = path.size();
    for (i = 0; i < size - 1; ++i) {
      w += matrix[path[i]][path[i+1]];
    }
  }

  return w;
}

unsigned Graph::weight(Path<int> &&path) {
  return weight(static_cast<const Path<int> &>(path));
}

unsigned Graph::weight(std::vector<int> path[maxnum][maxnum], unsigned s, unsigned t1, unsigned t2) {
  int w = 0;
  int i = 0;
  int size = 0;
  if (0 == t2) {
    if (path[s][t1].empty())
      w = matrix[s][t1];
    else {
      size = path[s][t1].size();
      for (i = 0; i < size - 1; ++i)
	w += matrix[path[s][t1][i]][path[s][t1][i+1]];
    }
  }
  else {
    size = path[s][t1].size();
    for (i = 0; i < size - 1; ++i)
      w += matrix[path[s][t1][i]][path[s][t1][i+1]];
    size = path[t1][t2].size();
    for (i = 0; i < size - 1; ++i)
      w += matrix[path[t1][t2][i]][path[t1][t2][i+1]];
  }
  return w;
}

void Graph::deriveGraph(const Graph &baseGraph, unsigned src, unsigned tgt) {
  if (src > tgt)
    std::swap(src, tgt);

  // baseGraph.printMatrix();

  // Copy the matrix from the base graph
  for (unsigned i = 0; i < baseGraph.matrix.size(); ++i)
    std::copy(baseGraph.matrix[i].begin(), baseGraph.matrix[i].end(), matrix[i].begin());

  if (tgt > baseGraph.getNum())
    matrix[tgt][tgt] = 0;

  // Add a new undirected edge src<-->tgt
  matrix[src][tgt] = 1;
  matrix[tgt][src] = 1;

  // Adjust all the possible paths
  deriveAllPossiblePaths(baseGraph, src, tgt);

  // Copy all the traces from the base graph
  // v_initTraces = baseGraph.v_traces;
  // for (unsigned i = 0; i < baseGraph.v_traces.size(); ++i)
  //   std::copy(baseGraph.v_traces[i].begin(), baseGraph.v_traces[i].end(), v_traces[i].begin());
  newSrc = src;
  newTgt = tgt;

  // if (tgt > baseGraph.getNum())
    return;
  // Copy all the trace points from the base graph
  // for (std::size_t i = 0; i < baseGraph.v_tracePoints.size(); ++i)
  //   for (std::size_t j = 0; j < baseGraph.v_tracePoints[i].size(); ++j)
  //     for (std::size_t k = 0; k < baseGraph.v_tracePoints[i][j].size(); ++k)
  // 	v_tracePoints[i][j].push_back(baseGraph.v_tracePoints[i][j][k]);
  // v_tracePoints = baseGraph.v_tracePoints;
}

void Graph::deriveGraph(Graph &&baseGraph, unsigned src, unsigned tgt) {
  deriveGraph(static_cast<const Graph &>(baseGraph), src, tgt);
}

void Graph::deriveAllPossiblePaths(const Graph &baseGraph, const unsigned &src, const unsigned &tgt) {
  auto baseNum = baseGraph.getNum();
  auto basePaths = baseGraph.v_possible_paths;
  if (tgt > baseNum) {

    // Add new node
    assert(src <= baseNum);
    for (unsigned i = 0; i < basePaths.size(); ++i) {
      // Copy all preserved paths
      std::copy(basePaths[i].begin(), basePaths[i].end(), v_possible_paths[i].begin());
      for (unsigned j = 0; j < basePaths[i].size(); ++j) {
	// v_possible_paths[i][j] = basePaths[i][j];
	v_possible_paths[tgt][j] = basePaths[src][j];
      }
      v_possible_paths[i][tgt] = basePaths[i][src];
    }
    maxLen = baseGraph.getMaxLen();

    for (unsigned i = 1; i < basePaths.size(); ++i) {
      // i-->src + src-->tgt => i-->tgt
      for (auto&& j : v_possible_paths[i][tgt]) {
	// j.path += "-";
	j.path.push_back(tgt);
	// j.path += std::to_string(tgt);
	j.nodes[tgt] = 1;
	if (j.nodes.count() > maxLen)
	  maxLen = j.nodes.count();
      }

      // tgt-->src + src-->i => tgt-->i
      for (auto&& j : v_possible_paths[tgt][i]) {
	String path;
	path.push_back(tgt);
	// path += "-";
	concat(path, j.path);
	j.path = path;
	j.nodes[tgt] = 1;
	if (j.nodes.count() > maxLen)
	  maxLen = j.nodes.count();
      }
    }

    StrPath strPath;
    strPath.path.push_back(tgt);
    strPath.nodes[tgt] = 1;
    v_possible_paths[tgt][tgt].push_back(strPath);
    // v_possible_paths[tgt][tgt][0].path = std::to_string(tgt);
    // v_possible_paths[tgt][tgt][0].nodes[tgt] = 1;
  }
  else {			// tgt == baseNum

    // Add new edge
    for (unsigned i = 0; i < basePaths.size(); ++i) {
      // Copy all preserved paths
      std::copy(basePaths[i].begin(), basePaths[i].end(), v_possible_paths[i].begin());      
    }
    maxLen = baseGraph.getMaxLen();

    for (unsigned i = 1; i < v_possible_paths.size(); ++i) {
      for (unsigned j = 0; j < v_possible_paths[i][src].size(); ++j) {
	for (unsigned k = 1; k < v_possible_paths[tgt].size(); ++k) {
	  for (unsigned l = 0; l < v_possible_paths[tgt][k].size(); ++l) {
	    // i-->src + src-->tgt + tgt-->k => i->k
	    std::bitset<maxnum> nodes = v_possible_paths[i][src][j].nodes & v_possible_paths[tgt][k][l].nodes;
	    if (nodes.none()) {
	      // if i-->x-->src + tgt-->x-->k does not exist
	      StrPath strPath;
	      concat(strPath.path, v_possible_paths[i][src][j].path);
	      concat(strPath.path, v_possible_paths[tgt][k][l].path);
	      strPath.nodes = v_possible_paths[i][src][j].nodes | v_possible_paths[tgt][k][l].nodes;
	      v_possible_paths[i][k].push_back(strPath);
	      if (strPath.nodes.count() > maxLen)
		maxLen = strPath.nodes.count();
	    }
	  }
	}
      }
    }

    // Just for undirected graph
    for (unsigned i = 1; i < v_possible_paths.size(); ++i) {
      for (unsigned j = 0; j < v_possible_paths[i][tgt].size(); ++j) {
	for (unsigned k = 1; k < v_possible_paths[src].size(); ++k) {
	  for (unsigned l = 0; l < v_possible_paths[src][k].size(); ++l) {
	    // i-->tgt + tgt-->src + src-->k => i->k
	    std::bitset<maxnum> nodes = v_possible_paths[i][tgt][j].nodes & v_possible_paths[src][k][l].nodes;
	    if (nodes.none()) {
	      // if i-->x-->tgt + src-->x-->k does not exist
	      StrPath strPath;
	      concat(strPath.path, v_possible_paths[i][tgt][j].path);
	      concat(strPath.path, v_possible_paths[src][k][l].path);
	      strPath.nodes = v_possible_paths[i][tgt][j].nodes | v_possible_paths[src][k][l].nodes;
	      v_possible_paths[i][k].push_back(strPath);
	      if (strPath.nodes.count() > maxLen)
		maxLen = strPath.nodes.count();
	    }
	  }
	}
      }
    }

  } // endif
}

// void Graph::copyBaseTraceInfoTree() {
  
// }

unsigned Graph::getMaxLen() const {
  return maxLen;
}

double Graph::getSum() const {
  return sum;
}

void Graph::setSum(const double &sum) {
  this->sum = sum;
}

unsigned Graph::getLastSrc() const {
  return lastSrc;
}

void Graph::setLastSrc(const unsigned &src) {
  lastSrc = src;
}

unsigned Graph::getLastTgt() const {
  return lastTgt;
}

void Graph::setLastTgt(const unsigned &tgt) {
  lastTgt = tgt;
}

unsigned Graph::getNewEdgesCounter() const {
  return newEdgesCounter;
}

void Graph::setNewEdgesCounter(const unsigned &newEdgesCounter) {
  this->newEdgesCounter = newEdgesCounter;
}

unsigned Graph::getFlagTgt() const {
  return flagTgt;
}

void Graph::setFlagTgt(const unsigned &tgt) {
  flagTgt = tgt;
}

void Graph::printMatrix() const {
    for(unsigned i=1; i<=_num; ++i) {
    for(unsigned j=1; j<=_num; ++j)
      printf("%12d", matrix[i][j]);
    printf("\n");
  }
}

// void Graph::clearInitTraces() {
//   for (unsigned i = 0; i < v_initTraces.size(); ++i) {
//     std::vector<TraceInfo> emptyVector;
//     v_initTraces[i].swap(emptyVector);
//   }
// }

unsigned Graph::getNewSrc() const {
  return newSrc;
}

unsigned Graph::getNewTgt() const {
  return newTgt;
}

void Graph::collectGarbage() {
  // for (auto &&i : v_traces)
  //   for (auto &&j : i)
  //     j.collectGarbage();
  // for (auto &&i : v_tracePoints)
  //   for (auto &&j : i)
  //     for (auto &&k : j)
  // 	k.collectGarbage();
  clear();
}

void Graph::clear() {
  // std::vector<std::vector<int> >().swap(matrix);
  // std::vector<std::vector<std::vector<StrPath> > >().swap(v_possible_paths);
  std::vector<std::vector<TraceInfo> >().swap(v_traces);
  std::vector<pthread_mutex_t>().swap(v_lock);
  std::vector<std::vector<std::deque<TraceInfo> > >().swap(v_tracePoints);
  // tbb::concurrent_vector<tbb::concurrent_vector<std::deque<TraceInfo> > >().swap(v_tracePoints);
  std::vector<std::vector<pthread_mutex_t> >().swap(v_psLock);
}

void Graph::swap(Graph &other) {
  std::vector<std::vector<int> >(other.matrix).swap(matrix);
  std::vector<std::vector<std::vector<StrPath> > >(other.v_possible_paths).swap(v_possible_paths);
  std::vector<std::vector<TraceInfo> >(other.v_traces).swap(v_traces);
  std::vector<pthread_mutex_t>(other.v_lock).swap(v_lock);
  std::vector<std::vector<std::deque<TraceInfo> > >(other.v_tracePoints).swap(v_tracePoints);
  // tbb::concurrent_vector<tbb::concurrent_vector<std::deque<TraceInfo> > >(other.v_tracePoints).swap(v_tracePoints);
  std::vector<std::vector<pthread_mutex_t> >(other.v_psLock).swap(v_psLock);
  std::swap(_num, other._num);
  std::swap(maxLen, other.maxLen);
  std::swap(sum, other.sum);
  std::swap(lastSrc, other.lastSrc);
  std::swap(lastTgt, other.lastTgt);
  std::swap(newEdgesCounter, other.newEdgesCounter);
  std::swap(flagTgt, other.flagTgt);
  std::swap(newSrc, other.newSrc);
  std::swap(newTgt, other.newTgt);
}
