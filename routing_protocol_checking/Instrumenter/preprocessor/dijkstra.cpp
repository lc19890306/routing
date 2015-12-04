#include "dijkstra.h"
#include "checkpoint.hh"

void initialize(Path<int> path[maxnum][maxnum], int n) {
  if (n == 0)
    n = maxnum;
  int i, j;
  for (i = 1; i <= n; ++i) {
    for (j = 1; j <= n; ++j) {
      path[i][j].set_head(i);
      path[i][j].set_rear(j);
    }
  }
}

void Dijkstra(const TraceInfo &traceInfo, Graph *graph) {
  const unsigned n = traceInfo.getLast();
  const unsigned v = traceInfo.getFirst();

  int res(0);
  TraceInfo this_traceInfo(traceInfo);
  unsigned i(0), j(0);
  Path<int> path[maxnum][maxnum];
  initialize(path, n);
  bool s[maxnum] = {false};    // 判断是否已存入该点到S集合中
  unsigned u(0);
  unsigned x(0), y(0);
  Path<int> tmp;

  // switch (this_traceInfo.getLabel()) {
  // case 1:
  //   goto L1;
  // case 2:
  //   goto L2;
  // case 3:
  //   goto L3;
  // case 4:
  //   goto L4;
  // case 5:
  //   goto L5;
  // case 6:
  //   goto L6;
  // case 7:
  //   goto L7;
  // default:
  //   break;
  // }

  for(i=1; i<=n; ++i) {
    if (v == i)
      continue;

    if (0 != compare(inf, graph->weight(path[v][i]))) {
      path[v][i].push_back(v);
      path[v][i].push_back(i);
    }
  }

  s[v] = 1;
    
  // 依次将未放入S集合的结点中，取dist[]最小值的结点，放入结合S中
  // 一旦S包含了所有V中顶点，dist就记录了从源点到所有其他顶点之间的最短路径长度
  // 注意是从第二个节点开始，第一个为源点
  for(i=2; i<=n; ++i) {
    u = v;

    for (x = 1; x <= n; ++x) {
      if (s[x])
    	continue;

      if (1 != compare(inf, graph->weight(path[v][x])))
    	continue;
      for (y = x + 1; y <= n; ++y) {
    	if (s[y])
    	  continue;

    	if (1 != compare(inf, graph->weight(path[v][y])))
    	  continue;

	res = compare(path[v][x], path[v][y], this_traceInfo, graph);
	if (2 == res)
	  return;
    	if (1 == res) {
    	  u = y;
    	  x = y;
    	  break;
    	}
      }
      if (x != y) {
    	u = x;
  	break;
      }
      else {
  	--x;
      }
    }

    s[u] = 1;    // 表示u点已存入S集合中
    // 更新dist
    for(j=1; j<=n; ++j) {
      if ((!s[j]) && -1 == compare(graph->weight(path[u][j]), inf)) {
  	path[u][j].push_back(u);
  	path[u][j].push_back(j);
	tmp.clear();
        tmp = path[v][u] + path[u][j];
	if (path[v][j].is_checked())
	  tmp.check();

	res = compare(tmp, path[v][j], this_traceInfo, graph);
	if (2 == res)
	  return;
  	if(-1 == res) {
  	  path[v][j].clear();
  	  path[v][j] = tmp;
  	}
      }
    }
  }

  if (v != n) {
    // for (unsigned ele = 0; ele != path[v][n].size(); ++ele)
    //   std::cout << path[v][n][ele];
    // std::cout << std::endl;
    this_traceInfo.setOptimalPath(assemble(path[v][n]));
    res = 0;
    // for (i = 0; i < traces.size(); ++i) {
    // rDB db = traces[i].db;

    // string optimalPath = traces[i].optimalPath;
    // cout << optimalPath << endl;
    // traces[i].db.print();
    auto size = graph->v_possible_paths[v][n].size();
    rDB db = rDB();
    for (i = 0; i < size; ++i) {
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

	  static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	  pthread_mutex_lock(&lock);
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
	  pthread_mutex_unlock(&lock);
	  exit(0);
	  return;
	}
      }
    }
  }
  pthread_mutex_lock(&graph->v_lock[v]);
  // this_traceInfo.getDB().print();
  graph->v_traces[v].push_back(this_traceInfo);
  pthread_mutex_unlock(&graph->v_lock[v]);
}

// void *thread_routine(void *arg) {
  // pthread_detach(pthread_self());
  // ck_pr_inc_64((uint64_t *)&runningThread::counter);
  // pthread_mutex_lock(&runningThread::lock);
  // ++runningThread::counter;
  // pthread_mutex_unlock(&runningThread::lock);

  // for (int i = 0; i < (*(Params *)arg).traces.size()
  // Params *params = NULL;
  // params = (Params *)arg;
  // Params params = *(static_cast<Params *>(arg));
  // Params params = *(Params *)arg;
  // Params *params = (Params *)arg;
  // int n = params->n;
  // int v = params->v;
  // int c[maxnum][maxnum];
  // for (int i = 0; i < maxnum; ++i)
  //   for (int j = 0; j < maxnum; ++j)
  //     c[i][j] = params->c[i][j];
  // TraceInfo this_traceInfo = params->traceInfo;
  // vector<TraceInfo> traces = params->traces;
  // vector<string> possible_paths = params->possible_paths;
  // int **c
  // delete (Params *)arg;
  // delete params;		// new in createParams

  // #############################################
  // pthread_mutex_lock(params.lock);
  //   pthread_cond_signal(params.cond);
  // pthread_mutex_unlock(params.lock);
  // pthread_cond_destroy(params.cond);
  // pthread_mutex_destroy(params.lock);
  // ##############################################

  // delete (Params *)arg;
  // Dijkstra(params.traceInfo);
  // Dijkstra(n, v, c, this_traceInfo, traces, possible_paths);
  // pthread_exit(NULL);

  // ck_pr_dec_64((uint64_t *)&runningThread::counter);
  // pthread_mutex_lock(&runningThread::lock);
  // --runningThread::counter;
  // pthread_mutex_unlock(&runningThread::lock);
//   return (void *)0;
// }

// Params createParams(const int &n, const int &v, const int c[maxnum][maxnum], const TraceInfo &traceInfo, const vector<string> &possible_paths) {
//   Params res;	// delete in thread_routine
//   res.n = n;
//   res.v = v;
//   for (int i = 0; i < maxnum; ++i)
//     for (int j = 0; j < maxnum; ++j)
//       res.c[i][j] = c[i][j];
//   res.traceInfo = traceInfo;
//   // res.traces = traces;
//   res.possible_paths = possible_paths;
//   static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//   static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//   // static bool flag = false;
//   res.cond = &cond;
//   res.lock = &lock;
//   // pthread_cond_init(&res.cond, NULL);
//   // pthread_mutex_init(&res.lock, NULL);
//   // res.flag = &flag;

//   return res;
// }

// Params *createParams(const TraceInfo &traceInfo) {
  // pthread_mutex_lock(&lock);
  // Params *res = new Params;	// delete in thread_routine
  // cout << pthread_self() << " : " << res << endl;
  // res->n = n;
  // res->v = v;

  // res->traceInfo = traceInfo;
  // res->traces = traces;
  // res->possible_paths = possible_paths;

  // #################################################
  // pthread_mutex_t *lock = new pthread_mutex_t;
  // pthread_cond_t *cond = new pthread_cond_t;
  // pthread_mutex_init(lock, NULL);
  // pthread_cond_init(cond, NULL);
  // res->cond = cond;
  // res->lock = lock;
  // #################################################

//   return res;
// }
