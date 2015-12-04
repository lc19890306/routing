/***************************************
 * About:    A routing protocol implementation based on Dijkstra's algorithm
 * Author:   Chang Liu
 ***************************************/

#include "check.hh"
// #include <unistd.h>

// std::vector<std::vector<int> > Graph::matrix = std::vector<std::vector<int> >(0);
// std::vector<std::vector<std::vector<StrPath> > > Graph::v_possible_paths = std::vector<std::vector<std::vector<StrPath> > >(0);
// vector<vector<TraceInfo> > Graph::v_traces = vector<vector<TraceInfo> >(0);
// std::vector<pthread_mutex_t> Graph::v_lock = std::vector<pthread_mutex_t>(0);

void check(pAlgoFunc algoFunc, Graph &graph) {
  const auto n = graph.getNum();
  unsigned i, j; 

  graph.printMatrix();
  std::puts("\n");
  // usleep(1000);

// #if 0
  if (2 == n) {
    for (i = 1; i <= n; ++i) {
      TraceInfo traceInfo(i, n);
      AlgoJob *job = new AlgoJob(algoFunc, traceInfo, &graph);
      ThreadPool::run(job, NULL, true);
    }
  }
  else {
    if (graph.v_tracePoints[graph.getNewSrc()][graph.getNewTgt()].empty()) {
      for (i = 1; i <= n; ++i) {
  	TraceInfo traceInfo(i, n);
  	AlgoJob *job = new AlgoJob(algoFunc, traceInfo, &graph);
  	ThreadPool::run(job, NULL, true);
      }
    }
    else {
      while (!graph.v_tracePoints[graph.getNewSrc()][graph.getNewTgt()].empty()) {
	TraceInfo traceInfo(graph.v_tracePoints[graph.getNewSrc()][graph.getNewTgt()].front());
	// if (traceInfo.getLast() < n)
	//   traceInfo.setLast(n);
	graph.v_tracePoints[graph.getNewSrc()][graph.getNewTgt()].pop_front();
	AlgoJob *job = new AlgoJob(algoFunc, traceInfo, &graph);
	ThreadPool::run(job, NULL, true);
      }
      std::deque<TraceInfo>().swap(graph.v_tracePoints[graph.getNewTgt()][graph.getNewSrc()]);

      while (!graph.v_tracePoints[graph.getNewTgt()][graph.getNewSrc()].empty()) {
	TraceInfo traceInfo(graph.v_tracePoints[graph.getNewTgt()][graph.getNewSrc()].front());
	graph.v_tracePoints[graph.getNewTgt()][graph.getNewSrc()].pop_front();
	AlgoJob *job = new AlgoJob(algoFunc, traceInfo, &graph);
	ThreadPool::run(job, NULL, true);
      }
      std::deque<TraceInfo>().swap(graph.v_tracePoints[graph.getNewTgt()][graph.getNewSrc()]);
    }
  }
      
  // while (true) {
  //   bool empty = true;

  //   for (i = 1; i <= n; ++i) {
  //     if (0 == TraceInfoQueue::v_queue[i].size_approx()) {
  // 	continue;
  //     }
  //     empty = false;
  //     TraceInfo traceInfo;
  //     TraceInfoQueue::v_queue[i].try_dequeue(traceInfo);

  //     AlgoJob *job = new AlgoJob(algoFunc, traceInfo, &graph);
  //     ThreadPool::run(job, NULL, true);
  //   }
  //   if (ck_pr_load_64(&runningThread::counter) == 0 && empty == true)
  //     break;
  // }
// #endif
  ThreadPool::sync_all();

  // usleep(1000);
  // cout << "insert counter: " << Counters::insert_cnt << endl;
  // cout << "query counter: " << Counters::query_cnt << endl;
  // cout << "query counter - insert counter: " << Counters::query_cnt - Counters::insert_cnt << endl;
  // for (i = 1; i <= n; ++i) {
  //   cout << graph.v_traces[i].size() << endl;
  // }

  // checkConsistency(algoFunc, graph);

  // MPI::Finalize();
  // if (rank == 0);
  // for (i = 1; i < n; ++i) {
  //   cout << Traces::_vector[i].size() << endl;
  // }

  /*
    for (i = 1; i < n; ++i) {
    vector<TraceInfo> traces1 = v_traces[i];
    vector<TraceInfo>::iterator iter1, iter2;
    for (iter1 = traces1.begin(); iter1 != traces1.end(); ++iter1) {
    string path1 = iter1->optimalPath;
    vector<string> v_path1 = split(path1);
    for (j = 1; j < v_path1.size() - 1; ++j) {
    int node = atoi(v_path1[j].c_str());
    vector<TraceInfo> traces2 = v_traces[node];
    for (iter2 = traces2.begin(); iter2 != traces2.end(); ++iter2) {
    rDB db1 = iter1->db;
    string path2 = iter2->optimalPath;
    rDB db2 = iter2->db;
    vector<string> v_path2 = split(path2);

    int size = v_possible_paths[i][n].size();
    for (int k = 0; k < size; ++k) {
    if (path1 != v_possible_paths[i][n][k]) {
    db2.query(path1, v_possible_paths[i][n][k]);
    }	    
    }

    if (endsWith(path1, path2)) {
    // db1.print();cout << endl;
    int res = db1.force_merge(db2);
    if (0 == res)
    cout << "consistent && loop-free\n";
    else {
    // vector<string> v1;
    // vector<string> v2 = v_path2;
    // vector<string> intersection;
    // string target = v_path2[0];
    // for (int i = 0; i < v_path1.size(); ++i) {
    // 	if (v_path1[i] != target)
    // 	  v1.push_back(v_path1[i]);
    // 	else
    // 	  break;
    // }
    // sort(v1.begin(), v1.end());
    // sort(v2.begin() + 1, v2.end());
    // // for (int i = 0; i < v1.size())
    // set_intersection(v1.begin(), v1.end(),
    // 		       v2.begin() + 1, v2.end(),
    // 		       inserter(intersection, intersection.begin()));
    // if (0 != intersection.size())
    // 	continue;
    TraceInfo traceInfo;
    vector<TraceInfo> temp;
    int node = atoi(v_path2[0].c_str());
    // db1.print();
    traceInfo.db = db1;
    algoFunc(n, node, c, traceInfo, temp, v_possible_paths[node][n]);
    // cout << temp.size() << endl;
    // db2.print();
    // cout << endl;
    if (temp[0].optimalPath == path2 && 0 == db1.merge(temp[0].db)) {
    cout << "correct\n";
    continue;
    }
    cout << "inconsistent1\n";
    cout << path1 << " " << path2 << endl;
    db1.print();
    cout << endl;
    db2.print();exit(0);
    }
    continue;
    }
    if (find(v_path2.begin(), v_path2.end(), v_path1[0]) != v_path2.end()) {
    int res = db1.merge(db2);
    if (0 == res) {
    // vector<string> v1;
    // vector<string> v2 = v_path2;
    // vector<string> intersection;
    // string target = v_path2[0];
    // for (int i = 0; i < v_path1.size(); ++i) {
    // 	if (v_path1[i] != target)
    // 	  v1.push_back(v_path1[i]);
    // 	else
    // 	  break;
    // }
    // sort(v1.begin(), v1.end());
    // sort(v2.begin() + 1, v2.end());
    // // for (int i = 0; i < v1.size())
    // set_intersection(v1.begin(), v1.end(),
    // 		       v2.begin() + 1, v2.end(),
    // 		       inserter(intersection, intersection.begin()));
    // if (0 != intersection.size())
    // 	continue;
    cout << "inconsistent && NOT loop-free\n";
    for(int row=1; row<=n; ++row) {
    for(int col=1; col<=n; ++col)
    printf("%8d", c[row][col]);
    printf("\n");
    }
    cout << endl;
    cout << path1 << endl;
    for (int k = 0; k < v_possible_paths[i][n].size(); ++k)
    if (v_possible_paths[i][n][k] != path1)
    cout << v_possible_paths[i][n][k] << endl;
    cout << endl;
    cout << path2 << endl;
    for (int k = 0; k < v_possible_paths[j][n].size(); ++j)
    if (v_possible_paths[j][n][k] != path2)
    cout << v_possible_paths[j][n][k] << endl;
    cout << "counterexample:\n";
    db1.print();
    cout << endl;
    // db2.print();
    exit(0);
    }
    else
    ;
    continue;
    }
    {
    int res = db1.merge(db2);
    if (0 == res) {
    vector<string> v1;
    vector<string> v2 = v_path2;
    vector<string> intersection;
    string target = v_path2[0];
    for (int i = 0; i < v_path1.size(); ++i) {
    if (v_path1[i] != target)
    v1.push_back(v_path1[i]);
    else
    break;
    }
    sort(v1.begin(), v1.end());
    sort(v2.begin() + 1, v2.end());
    // for (int i = 0; i < v1.size())
    set_intersection(v1.begin(), v1.end(),
    v2.begin() + 1, v2.end(),
    inserter(intersection, intersection.begin()));
    if (0 != intersection.size())
    continue;
    cout << "inconsistent2\n";
    // cout << path1 << " " << path2 << endl;
    // db1.print();
    // cout << endl;
    // db2.print();
    // exit(0);
    }
    else
    ;
    }
    }
    }
    }
    }
  */
  // string<string> possible_paths = dft(n, 2, c);
  // for (int i = 0; i < possible_paths.size(); ++i)
  //   cout << possible_paths[i] << endl;

  // 最短路径长度
  // printf("源点到最后一个顶点的最短路径长度: %d\n", dist[n]);
  // int size = path[1][n].size();
  // int sum = 0;
  // for (i = 0; i < size - 1; ++i) {
  //   cout << path[1][n][i] << " -> ";
  //   sum += c[path[1][n][i]][path[1][n][i+1]];
  // }cout << path[1][n][i] << endl;
  // cout << "源点到最后一个顶点的最短路径长度: " << sum << endl;


  // 路径
  /* printf("源点到最后一个顶点的路径为: "); */
  // cout << "源点到最后一个顶点的路径为: ";
  /* searchPath(paths, 1, n); */

  // cout << "Finally\n";
}
