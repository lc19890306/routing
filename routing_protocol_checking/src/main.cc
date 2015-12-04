#include "main.hh"

// std::vector<moodycamel::ConcurrentQueue<TraceInfo> > TraceInfoQueue::v_queue = std::vector<moodycamel::ConcurrentQueue<TraceInfo> >();
std::set<std::shared_ptr<void> > AddressBook::addresses = std::set<std::shared_ptr<void> >();
pthread_mutex_t AddressBook::lock = PTHREAD_MUTEX_INITIALIZER;
template <typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;
std::atomic<uint64_t> runningThread::counter(0);
bool runningThread::enq = true;

// uint64_t Counters::query_cnt = 0;
// uint64_t Counters::insert_cnt = 0;

int main(int argc, char **argv) {
  unsigned n = 2;
  if (argc > 1)
    n = atoi(argv[1]);

  // pAlgoFunc algoFunc(Bellman_Ford);
  pAlgoFunc algoFunc(Dijkstra);
  Singleton<pAlgoFunc>::instance() = algoFunc;

  // TraceInfoQueue::v_queue.resize(n + 1);

  int geng_argc(5);
  char *geng_argv[] = {"geng", "-lv", "-c", "-D4", "2", NULL};
  GENG_MAIN(geng_argc, geng_argv);

  // std::cout << "Checking optimality...\n";

  int thr_count = 32;
  ThreadPool::init(thr_count);

  std::vector<std::vector<std::deque<Graph> > > preservedGraphs(2, std::vector<std::deque<Graph> >(n + 1)); // preservedGraphs[i&1][j][k]: k-th preserved graphs with a max length of j and i nodes
  // std::array<std::vector<std::deque<Graph> >, 2> preservedGraphs{std::vector<std::deque<Graph> >(n + 1), std::vector<std::deque<Graph> >(n + 1)}; // preservedGraphs[i&1][j][k]: k-th preserved graphs with a max length of j and i nodes
  std::vector<std::deque<Graph> > derivedGraphs(n + 1); // derivedGraphs[i][j]: j-th graph with a max length of i

  std::cout << "Checking 2-node graphs...\n";
  // Check the graph of 2 nodes
  Graph graph(2);
  generate(graph.matrix, 2);
  for (int i = 1; i < 3; ++i) {
    for (int j = 1; j < 3; ++j)
      // graph.v_possible_paths[i][j] = dft(j, i, graph);
      dft(j, i, graph);
  }
  check(algoFunc, graph);
  graph.collectGarbage();
  preservedGraphs[2 & 1][2].push_back(graph);
  // std::cout << "No bug found in all 2-node graphs.\n";

  for (unsigned i = 3; i <= n; ++i) {
    std::cout << "Checking " << i << "-node graphs...\n";

    geng_argv[4] = std::to_string(i).c_str();
    GENG_MAIN(geng_argc, geng_argv);
    // check 1<-->i
    Graph graph(i);
    graph.deriveGraph(preservedGraphs[(i & 1) ^ 1][i - 1][0], i - 1, i);
    check(algoFunc, graph);
    graph.setLastSrc(1);
    graph.setLastTgt(2);
    graph.setFlagTgt(2);
    graph.setSum(0);
    graph.collectGarbage();
    preservedGraphs[i & 1][i].push_back(graph);
// #if 0
    // derive from 1<-->i
    unsigned totalNewEdges = ((i - 1) * (i - 2)) >> 1;
    for (unsigned newEdgesCounter = 1; newEdgesCounter <= totalNewEdges; ++newEdgesCounter) {
      do {
	Graph baseGraph;
	if (1 == newEdgesCounter)
	  baseGraph = preservedGraphs[i & 1][i][0];
	else
	  baseGraph = derivedGraphs[i][0];

	// Derive from the base graph
	for (auto src = baseGraph.getLastSrc(), tgt = baseGraph.getLastTgt() + 1; src < i - 1; ++tgt) {
	  if (tgt > i) {
	    ++src;
	    tgt = src + 1;
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
	  graph.collectGarbage();
	  derivedGraphs[i].push_back(graph);
	}
	// Pop corresponding base graphs
	if (newEdgesCounter > 1) {
	  derivedGraphs[i].front().collectGarbage();
	  derivedGraphs[i].pop_front();
	}
      } while (derivedGraphs[i][0].getNewEdgesCounter() < newEdgesCounter);
    }
    // Pop all derived graphs
    while (!derivedGraphs[i].empty()) {
      derivedGraphs[i].front().collectGarbage();
      derivedGraphs[i].pop_front();
    }
    std::deque<Graph>().swap(derivedGraphs[i]);

    if (3 == i) {
      for (unsigned index = 0; index < preservedGraphs[(i & 1) ^ 1].size(); ++index) {
      	while (!preservedGraphs[(i & 1) ^ 1][index].empty()) {
	  preservedGraphs[(i & 1) ^ 1][index].front().collectGarbage();
	  preservedGraphs[(i & 1) ^ 1][index].pop_front();
	}
	std::deque<Graph>().swap(preservedGraphs[(i & 1) ^ 1][index]);
      }
      continue;
    }

    // Check all graphs with a maximum length of less than i
    unsigned index;
    for (unsigned maxLen = i - 1; maxLen > 2; --maxLen) {
      index = 0;
      while (!preservedGraphs[(i & 1) ^ 1][maxLen].empty()) {
	auto totalNewEdges = i - 1 - preservedGraphs[(i & 1) ^ 1][maxLen][0].getFlagTgt();
	auto baseCounter = preservedGraphs[(i & 1) ^ 1][maxLen][0].getNewEdgesCounter();
	for (unsigned newEdgesCounter = 1; newEdgesCounter <= totalNewEdges; ++newEdgesCounter) {
	  do {
	    Graph baseGraph;
	    unsigned begin_tgt;
	    if (1 == newEdgesCounter) {
	      baseGraph = preservedGraphs[(i & 1) ^ 1][maxLen][0];
	      begin_tgt = baseGraph.getFlagTgt();
	    }
	    else {
	      if (index >= preservedGraphs[i & 1][maxLen].size())
		break;
	      baseGraph = preservedGraphs[i & 1][maxLen][index];
	      begin_tgt = baseGraph.getLastTgt();
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
	      graph.matrix[1][i] = 0;
	      graph.matrix[i][maxLen] = 0;
	      graph.matrix[maxLen][i] = 0;
	      check(algoFunc, graph);
	      graph.setSum(sum);
	      graph.setLastSrc(src);
	      graph.setLastTgt(tgt);
	      graph.setFlagTgt(baseGraph.getFlagTgt());
	      graph.setNewEdgesCounter(baseGraph.getNewEdgesCounter() + 1);
	      graph.collectGarbage();
	      preservedGraphs[i & 1][maxLen].push_back(graph);

	      checkDerivedGraphs(algoFunc, graph, maxLen, (avg == median));
	    }
	    if (1 == newEdgesCounter) {
	      preservedGraphs[(i & 1) ^ 1][maxLen].front().collectGarbage();
	      preservedGraphs[(i & 1) ^ 1][maxLen].pop_front();
	      // index = 0;
	    }
	    else
	      ++index;
	  } while (preservedGraphs[i & 1][maxLen][index].getNewEdgesCounter() < (baseCounter + newEdgesCounter));
	}
      }
    }
    while (!derivedGraphs[i].empty()) {
      derivedGraphs[i].front().collectGarbage();
      derivedGraphs[i].pop_front();
    }
    std::deque<Graph>().swap(derivedGraphs[i]);
    for (unsigned maxLen = 2; maxLen < preservedGraphs[(i & 1) ^ 1].size(); ++maxLen) {
      while (!preservedGraphs[(i & 1) ^ 1][maxLen].empty()) {
	preservedGraphs[(i & 1) ^ 1][maxLen].front().collectGarbage();
	preservedGraphs[(i & 1) ^ 1][maxLen].pop_front();
	std::deque<Graph>().swap(preservedGraphs[(i & 1) ^ 1][maxLen]);
      }
    }
// #endif
    // std::cout << "No bug found in all " << i << "-node graphs.\n";
  }

  ThreadPool::done();

  // Garbage collection
  for (auto &&x : derivedGraphs)
    for (auto &&y : x)
      y.collectGarbage();
  for (auto &&x : preservedGraphs)
    for (auto &&y : x)
      for (auto &&z : y)
  	z.collectGarbage();

  return 0;
}

void checkDerivedGraphs(pAlgoFunc algoFunc, const Graph &inputGraph, const unsigned &maxLen, const bool &val) {
  // if (true == val)
  //   std::cout << "avg == median\n";
  // else
  //   std::cout << "avg < median\n";

  std::deque<Graph> derivedGraphs;
  unsigned totalNewEdges = ((maxLen - 1) * (maxLen - 2)) >> 1;
  for (unsigned newEdgesCounter = 1; newEdgesCounter <= totalNewEdges; ++newEdgesCounter) {
    do {
      Graph baseGraph;
      if (1 == newEdgesCounter) {
	baseGraph = inputGraph;
	baseGraph.setLastSrc(1);
	baseGraph.setLastTgt(2);
	baseGraph.setSum(0);
      }
      else
	baseGraph = derivedGraphs[0];

      // Derive from the base graph
      for (auto src = baseGraph.getLastSrc(), tgt = baseGraph.getLastTgt() + 1; src < maxLen - 1; ++tgt) {
	if (tgt > maxLen) {
	  ++src;
	  tgt = src + 1;
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
	graph.collectGarbage();
	derivedGraphs.push_back(graph);
      }
      // Pop corresponding base graphs
      if (newEdgesCounter > 1) {
	derivedGraphs.front().collectGarbage();
	derivedGraphs.pop_front();
      }
    } while (derivedGraphs[0].getNewEdgesCounter() < newEdgesCounter);
  }
}
