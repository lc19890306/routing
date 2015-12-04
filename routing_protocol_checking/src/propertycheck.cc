#include "propertycheck.hh"

void checkOptimality(Graph *graph, TraceInfo &this_traceInfo, const Path<int> &path) {
  const auto v = this_traceInfo.getFirst();
  const auto n = this_traceInfo.getLast();
  if (v != n) {
    // for (unsigned ele = 0; ele != path[v][n].size(); ++ele)
    //   std::cout << path[v][n][ele];
    // std::cout << std::endl;
    this_traceInfo.setOptimalPath(assemble(path));
    int res(0);
    // for (i = 0; i < traces.size(); ++i) {
    // rDB db = traces[i].db;

    // string optimalPath = traces[i].optimalPath;
    // cout << optimalPath << endl;
    // traces[i].db.print();
    auto size = graph->v_possible_paths[v][n].size();
    rDB db = rDB();
    for (std::size_t i = 0; i < size; ++i) {
      // rDB db = this_traceInfo.db;
      // db.print();
      if (this_traceInfo.getOptimalPath() != graph->v_possible_paths[v][n][i].path) {
	// static pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
	// if (this_traceInfo.getOptimalPath() == "1-4" && possible_paths[i] == "1-2-3-4") {
	// 	pthread_mutex_lock(&lock1);
	// 	this_traceInfo.getDB().print();
	// 	pthread_mutex_unlock(&lock1);
	// }
	db = this_traceInfo.getDB();
	res = db.query(this_traceInfo.getOptimalPath(), graph->v_possible_paths[v][n][i].path);
	if (2 == res)
	  return;
	// usleep(100);
	this_traceInfo.setDB(db);
	// res = this_traceInfo.getDB().query(this_traceInfo.getOptimalPath(), possible_paths[i]);
	// db.print();
	if (res == 0) {
	  // db.print();
	  // for(int row=1; row<=n; ++row) {
	  //   for(int col=1; col<=n; ++col)
	  //     printf("%8d", c[row][col]);
	  //   printf("\n");
	  // }
	  // cout << endl;
	  // this_traceInfo.db.insert(this_traceInfo.optimalPath, possible_paths[i], 1);

	  static std::mutex scoped_mutex;
	  std::lock_guard<std::mutex> scoped_lock(scoped_mutex);
	  // static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	  // pthread_mutex_lock(&lock);
	  // this_traceInfo.getDB().print();
	  // cout << endl;
	  db = this_traceInfo.getDB();
	  db.print();
	  res = db.insert(this_traceInfo.getOptimalPath(), graph->v_possible_paths[v][n][i].path, 1);
	  if (2 == res)
	    return;
	  this_traceInfo.setDB(db);
	  // this_traceInfo.getDB().insert(this_traceInfo.getOptimalPath(), possible_paths[i], 1);
	  cout << this_traceInfo.getOptimalPath() << " -vs- " << graph->v_possible_paths[v][n][i].path << " : " << res << endl << endl;
	  cout << "counterexample:\n";
	  this_traceInfo.getDB().print();
	  cout << endl;
	  // MPI_Finalize();
	  // pthread_exit(NULL);
	  // pthread_mutex_unlock(&lock);
	  exit(0);
	  return;
	}
      }
    }
  }
}

void checkConsistency(const pAlgoFunc &algoFunc, const Graph &graph) {
  const auto n = graph.getNum();
  for (std::size_t i = 1; i != n; ++i) {
    auto traces1(graph.v_traces[i]);
    std::vector<TraceInfo>::iterator iter1;
    for (iter1 = traces1.begin(); iter1 != traces1.end(); ++iter1) {
      while (runningThread::counter >= 8)
	checkConsistency(algoFunc, graph, *iter1, i);
      // 	;
      CheckConsistencyJob *job = new CheckConsistencyJob(algoFunc, graph, *iter1, i);
      ThreadPool::run(job, NULL, true);
    }
  }

  ThreadPool::sync_all();
}

void checkConsistency(const pAlgoFunc &algoFunc, const Graph &graph, const TraceInfo &traceInfo, const std::size_t &i) {
  const auto n = graph.getNum();
  auto path1(traceInfo.getOptimalPath());
  if (path1.empty()) {
    // std::cout << "i = " << i << " n = " << n << std::endl;
    return;
  }
  // auto v_path1(split(path1));
  for (std::size_t j = 1; j != path1.size() - 1; ++j) {
    // auto node = atoi(v_path1[j].c_str());
    // auto traces2 = graph.v_traces[node];
    auto traces2(graph.v_traces[path1[j]]);
    for (std::vector<TraceInfo>::iterator iter2 = traces2.begin(); iter2 != traces2.end(); ++iter2) {
      // const auto db1 = traceInfo.getDB();
      auto db1 = traceInfo.getDB();
      auto path2 = iter2->getOptimalPath();
      auto db2 = iter2->getDB();
      // auto v_path2 = split(path2);

      // Make sure all databases have all the info
      auto size = graph.v_possible_paths[i][n].size();
      for (std::size_t k = 0; k != size; ++k) {
	if (path1 != graph.v_possible_paths[i][n][k].path) {
	  db2.query(path1, graph.v_possible_paths[i][n][k].path);
	}	    
      }

      if (endsWith(path1, path2)) {
	// db1.print();cout << endl;
	rDB db(db1);
	auto res = db1.force_merge(db2);
	if (0 == res)
	  ;
	// cout << "consistent && loop-free\n";
	else {
	  // Double check if merging fails
	  // auto node = atoi(v_path2.front().c_str());
	  // db1.print();
	  TraceInfo tempTraceInfo(path2.front(), n);
	  tempTraceInfo.setDB(db1);
	  Graph tempGraph(graph);
	  tempGraph.v_traces[path2.front()].clear();
	  runningThread::enq = false;
	  algoFunc(tempTraceInfo, &tempGraph);
	  // cout << temp.size() << endl;
	  // db2.print();
	  // cout << endl;

	  bool flag(false);
	  for (auto &&element : tempGraph.v_traces[path2.front()])
	    if (element.getOptimalPath() == path2 && 0 == db1.merge(element.getDB())) {
	      // std::cout << "correct\n";
	      flag = true;
	      break;
	    }
	  if (flag)
	    continue;
	  std::cout << "inconsistent1\n";
	  std::cout << path1 << " " << path2 << std::endl;
	  db1.print();
	  std::cout << std::endl;
	  db2.print();exit(0);
	}
	continue;
      }

      // If path1 does not end with path2
      // Finding possible loops (e.g., 1-2-3 and 2-1-3)
      if (find(path2.begin(), path2.end(), path1.front()) != path2.end()) {
	// rDB db(db1);
	auto res = db1.merge(db2);
	if (0 == res) {
	  String v1;
	  auto v2(path2);
	  String intersection;
	  auto target = path2[0];
	  for (std::size_t i = 0; i != path1.size(); ++i) {
	    if (path1[i] != target)
	      v1.push_back(path1[i]);
	    else
	      break;
	  }
	  sort(v1.begin(), v1.end());
	  sort(v2.begin() + 1, v2.end());
	  // for (int i = 0; i < v1.size())
	  set_intersection(v1.begin(), v1.end(),
			   v2.begin() + 1, v2.end(),
			   std::inserter(intersection, intersection.begin()));
	  if (0 != intersection.size())
	    continue;
	  std::cout << "inconsistent && NOT loop-free\n";
	  std::cout << path1 << " " << path2 << std::endl;
	  // db1.query(path1, "1-4-1-5");
	  // db1.print();
	  // cout << endl;
	  // db2.print();
	  exit(0);
	}
	else
	  ;
	continue;
      }
      {
	rDB db(db1);
	auto res = db1.merge(db2);
	if (0 == res) {
	  String v1;
	  auto v2 = path2;
	  String intersection;
	  auto target = path2[0];
	  for (std::size_t i = 0; i < path1.size(); ++i) {
	    if (path1[i] != target)
	      v1.push_back(path1[i]);
	    else
	      break;
	  }
	  sort(v1.begin(), v1.end());
	  sort(v2.begin() + 1, v2.end());
	  // for (int i = 0; i < v1.size())
	  set_intersection(v1.begin(), v1.end(),
			   v2.begin() + 1, v2.end(),
			   std::inserter(intersection, intersection.begin()));
	  if (0 != intersection.size())
	    continue;
	  bool flag(true);
	  for (std::size_t k = 0; k != size; ++k) {
	    if (path1 != graph.v_possible_paths[i][n][k].path) {
	      if (-1 != db1.query(path1, graph.v_possible_paths[i][n][k].path)) {
		flag = false;
		break;
	      }
	    }	    
	  }
	  if (flag)
	    continue;
	  std::cout << "inconsistent2\n";
	  cout << path1 << " " << path2 << endl;
	  cout << endl;
	  db.print();
	  cout << endl;
	  rDB temp(db1);
	  temp.print();
	  cout << endl;
	  db2.print();
	  exit(0);
	}
	else
	  ;
      }
    }
  }
}
