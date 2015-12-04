#include "main.hh"

std::vector<moodycamel::ConcurrentQueue<TraceInfo> > TraceInfoQueue::v_queue = std::vector<moodycamel::ConcurrentQueue<TraceInfo> >();

int main(int argc, char **argv) {
  int n = 2;
  if (argc > 1)
    n = atoi(argv[1]);

  pAlgoFunc algoFunc = Dijkstra;

  TraceInfoQueue::v_queue.resize(n);

  int thr_count = 32;
  ThreadPool::init(thr_count);

  std::vector<std::vector<std::deque<Graph> > > preservedGraphs(2, std::vector<std::deque<Graph> >(n + 1)); // preservedGraphs[i&1][j][k]: k-th preserved graph with a max length of j and i nodes
  std::vector<std::deque<Graph> > derivedGraphs(n + 1); // derivedGraphs[i][j]: j-th graph with a max length of i

  // Check the graph of 2 nodes
  Graph graph(2);
  generate(graph.matrix, 2);
  for (unsigned i = 1; i < 3; ++i) {
    for (unsigned j = 1; j < 3; ++j)
    graph.v_possible_paths[i][j] = dft(j, i, graph);
  }
  check(algoFunc, graph);
  preservedGraphs[2 & 1][2].push_back(graph);








  // check 1<-->i
  i = n;			// start from 3
  Graph graph(i);
  graph.deriveGraph(preservedGraphs[(i - 1) & 1][i - 1][0], i - 1, i);
  check(algoFunc, graph);
  graph.setLastSrc(1);
  graph.setLastTgt(2);
  graph.setFlagTgt(2);
  graph.setSum(0);
  preservedGraphs[i & 1][i].push_back(graph);

  // derive from 1<-->i
  unsigned totalNewEdges = ((i - 1) * (i - 2)) >> 1;
  for (unsigned newEdgesCounter = 1; newEdgesCounter <= totalNewEdges; ++newEdgesCounter) {
    do {
      if (1 == newEdgesCounter)
	auto baseGraph = preservedGraphs[i & 1][i][0];
      else
	auto baseGraph = derivedGraphs[i][0];

      // Derive from the base graph
      for (src = baseGraph.getLastSrc(), tgt = baseGraph.getLastTgt() + 1; src < i - 1; ++tgt) {
	if (tgt > i) {
	  ++src;
	  tgt = src + 2;
	  continue;
	}
	auto sum = baseGraph.getSum() + src + tgt;
	if ((double)(sum / newEdgesCounter) > (i + 1))
	  continue;
	Graph graph(i);
	graph.deriveGraph(baseGraph, src, tgt);
	check(algoFunc, graph);
	graph.setSum(sum);
	graph.setLastSrc(src);
	graph.setLastTgt(tgt);
	graph.setNewEdgesCounter(newEdgesCounter);
	derivedGraphs[i].push_back(graph);
      }
      // Pop corresponding base graphs
      if (newEdgesCounter > 1)
	derivedGraphs[i].pop_front();
    } while (derivedGraphs[i][0].getNewEdgesCounter() < newEdgesCounter);
  }
  // Pop all derived graphs
  std::deque<int> emptyQueue;
  derivedGraphs[i].swap(emptyQueue);
  // while (!v[i].derivedGraphs[i].empty())
  //   v[i].derivedGraphs[i].pop_front();












  // i = n;			// start from 4
  // for (unsigned maxLen = i - 1; maxLen > 2; --maxLen) {
  //   while (!v[i - 1].preservedGraphs[maxLen].empty()) {
  //     auto baseGraph = v[i - 1].preservedGraphs[maxLen][0];
  //     for (unsigned src = i, tgt = baseGraph.getLastTgt(); tgt < i; ++tgt) {
  // 	if (tgt == maxLen)
  // 	  continue;
  // 	double avg_tgt = (double)(1 + maxLen) / 2;
  // 	auto sum = baseGraph.getSum();
  // 	if (tgt > maxLen)
  // 	  sum = sum + src + avg_tgt;
  // 	else
  // 	  sum = sum + src + tgt;
  // 	double avg = (double)sum / (i - maxLen);
  // 	double median = (double)(maxLen + 1 + i) / 2 + avg_tgt;
  // 	if (avg > median)
  // 	  continue;
  // 	Graph graph(i);
  // 	graph.deriveGraph(baseGraph, src, tgt);
  // 	if (graph.getMaxLen() > maxLen)
  // 	  continue;
  // 	graph.matrix[i][1] = 0;
  // 	graph.matrix[i][maxLen] = 0;
  // 	check(algoFunc, graph);
  // 	graph.setSum(sum);
  // 	graph.setLastSrc(src);
  // 	graph.setLastTgt(tgt);
  // 	v[i].preservedGraphs[maxLen].push_back(graph);
  //     }
  //     v[i - 1].preservedGraphs[maxLen].pop_front();
  //   }
  // }












  i = n;			// start from 4
  unsigned index;
  for (unsigned maxLen = i - 1; maxLen > 2; --maxLen) {
    while (!preservedGraphs[(i & 1) ^ 1][maxLen].empty()) {
      auto totalNewEdges = i - 1 - preservedGraphs[(i & 1) ^ 1][maxLen][0].getFlagTgt();
      auto baseCounter = preservedGraphs[(i & 1) ^ 1][maxLen][0].getNewEdgesCounter();
      for (unsigned newEdgesCounter = 1; newEdgesCounter <= totalNewEdges; ++newEdgesCounter) {
	do {
	  if (1 == newEdgesCounter) {
	    auto baseGraph = preservedGraphs[(i & 1) ^ 1][maxLen][0];
	    auto begin_tgt = baseGraph.getFlagTgt();
	  }
	  else {
	    auto baseGraph = preservedGraphs[i & 1][maxLen][index];
	    auto begin_tgt = baseGraph.getLastTgt();
	  }
	  for (unsigned src = i, tgt = begin_tgt; tgt < i; ++tgt) {
	    if (tgt == maxLen)
	      continue;
	    double avg_tgt = (double)(1 + maxLen) / 2;
	    auto sum = baseGraph.getSum();
	    if (tgt > maxLen)
	      sum = sum + src + avg_tgt;
	    else
	      sum = sum + src + tgt;
	    double avg = (double)sum / (baseGraph.getNewEdgesCounter() + 1);
	    double median = (double)(maxLen + 1 + i) / 2 + avg_tgt;
	    if (avg > median)
	      continue;
	    Graph graph(i);
	    graph.deriveGraph(baseGraph, src, tgt);
	    if (graph.getMaxLen() > maxLen)
	      continue;
	    graph.matrix[i][1] = 0;
	    graph.matrix[i][maxLen] = 0;
	    check(algoFunc, graph);
	    graph.setSum(sum);
	    graph.setLastSrc(src);
	    graph.setLastTgt(tgt);
	    graph.setFlagTgt(baseGraph.getFlagTgt());
	    graph.setNewEdgesCounter(baseGraph.getNewEdgesCounter() + 1);
	    preservedGraphs[i & 1][maxLen].push_back(graph);

	    checkDerivedGraphs(graph, maxLen, (avg == median));
	  }
	  if (1 == newEdgesCounter) {
	    preservedGraphs[(i & 1) ^ 1][maxLen].pop_front();
	    index = 0;
	  }
	  else
	    ++index;
	} while (preservedGraphs[i & 1][maxLen][index].getNewEdgesCounter() < (baseCounter + newEdgesCounter));
      }
    }
  }








  for (unsigned i = 3; i <= n; ++i) {
    Graph graph(i);
    check(algoFunc, graph);
  }

  ThreadPool::done();

  return 0;
}

void checkDerivedGraphs(const Graph &inputGraph, const unsigned &maxLen, const bool &val) {
  if (true == val)
    std::cout << "avg == median\n";
  else
    std::cout << "avg < median\n";

  std::deque<Graph> derivedGraphs;
  unsigned totalNewEdges = ((maxLen - 1) * (maxLen - 2)) >> 1;
  for (unsigned newEdgesCounter = 1; newEdgesCounter <= totalNewEdges; ++newEdgesCounter) {
    do {
      if (1 == newEdgesCounter) {
	auto baseGraph = inputGraph;
	baseGraph.setLastSrc(1);
	baseGraph.setLastTgt(2);
	baseGraph.setSum(0);
      }
      else
	auto baseGraph = derivedGraphs[0];

      // Derive from the base graph
      for (src = baseGraph.getLastSrc(), tgt = baseGraph.getLastTgt() + 1; src < i - 1; ++tgt) {
	if (tgt > i) {
	  ++src;
	  tgt = src + 2;
	  continue;
	}
	auto sum = baseGraph.getSum() + src + tgt;
	if (val && ((double)(sum / newEdgesCounter) > (maxLen + 1)))
	  continue;
	Graph graph(baseGraph.getNum());
	graph.deriveGraph(baseGraph, src, tgt);
	check(algoFunc, graph);
	graph.setSum(sum);
	graph.setLastSrc(src);
	graph.setLastTgt(tgt);
	graph.setNewEdgesCounter(newEdgesCounter);
	derivedGraphs.push_back(graph);
      }
      // Pop corresponding base graphs
      if (newEdgesCounter > 1)
	derivedGraphs.pop_front();
    } while (derivedGraphs[0].getNewEdgesCounter() < newEdgesCounter);
  }
}
