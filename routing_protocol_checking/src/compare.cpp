#include "compare.h"
#include <unistd.h>

// extern int c[maxnum][maxnum];
// extern int n;
// extern int line;
// extern int dist[maxnum];
// extern Path<int> path[maxnum][maxnum];

int compare(const int &a, const int &b) {
  return (a < b) ? -1 : ((a > b) ? 1 : 0);
}

int compare(const Path<int> &p1, const Path<int> &p2, TraceInfo &traceInfo, Graph *graph) {
  // const int v = traceInfo.getFirst();
  unsigned currentLevel = traceInfo.getLevel();
  ++currentLevel;
  traceInfo.setLevel(currentLevel);
  int res(0);// = traceInfo.getCmpVal(currentLevel);
  // if (res != 0)
  //   return res;
  if (0 == p1.size())
    return 1;
  if (0 == p2.size())
    return -1;
  auto path1(assemble(p1));
  auto path2(assemble(p2));

  rDB db = traceInfo.getDB();

#ifdef CGF
  if (!db.isMFF()) {		// if greedy routing is working
    TraceInfo copy(traceInfo);
    db = copy.getDB();
    db.visit(path1.back());
    db.visit(path2.back());
    if (db.is_all_visited())
      db.flipMFF();
    copy.setDB(db);
    pAlgoFunc algoFunc(Singleton<pAlgoFunc>::instance());
    if (runningThread::counter < 16 && runningThread::enq) {
      AlgoJob *job = new AlgoJob(algoFunc, copy, graph);
      ThreadPool::run(job, NULL, true);
      // TraceInfoQueue::v_queue[v].enqueue(copy);
    }
  }
  else {			// if MFF routing is working
    db.visit(path1.back());
    db.visit(path2.back());
    if (db.is_all_visited())
      db.flipMFF();
    traceInfo.setDB(db);
  }
#endif	// CGF

  res = db.query(path1, path2);
  if (2 == res)
    return 2;
  traceInfo.setDB(db);
  if (0 != res)
    return res;
  else {
    TraceInfo copy(traceInfo);
    db = copy.getDB();
    // db.print();
    res = db.insert(path1, path2, 1);
    if (2 == res)
      return 2;
    // db.print();
    copy.setDB(db);
    // copy.addCmpVal(1);
    copy.resetLevel();
    copy.setJob(RESUME);
    // copy.setTheLess(path2);
    // copy.setTheGreater(path1);

    // copy.getDB().insert(path1, path2, 1);
    /* add_right_node(p1, p2, 1); */
    // cout << copy.db.query(path1, path2) << endl;
    // size_t stackSize = 90000000;
    // pthread_attr_t attr;
    // pthread_attr_init(&attr);
    // pthread_attr_getstacksize(&attr, &stackSize);
    // static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    // pthread_mutex_lock(&lock);
    // cout << "Default stack size = " << stackSize << endl;
    // cout << sizeof(Params)*maxnum*maxnum + sizeof(TraceInfo) << endl;
    // pthread_mutex_unlock(&lock);
    // pthread_attr_setstacksize(&attr, stackSize);
    // Params params = createParams(n, v, c, copy, possible_paths);

    // ###############################################################
    // if (currentLevel < MAX_LEVEL) {
    pAlgoFunc algoFunc(Singleton<pAlgoFunc>::instance());
    if (runningThread::counter < 16 && runningThread::enq) {
      AlgoJob *job = new AlgoJob(algoFunc, copy, graph);
      ThreadPool::run(job, NULL, true);
      // TraceInfoQueue::v_queue[v].enqueue(copy);
    }
    // ###############################################################

    // cout << "I'm waiting\n";
    // *flag = false;
    // while (*(params.flag) == false)
    // cout << params.flag << " : " << *(params.flag) << endl;
    // cout << pthread_self() << " : " << lock << endl;
    // pthread_cond_destroy(&params.cond);
    // pthread_mutex_destroy(&params.lock);
    // pthread_join(thread_id, NULL);
    // sleep(5);
    // if (status) {
    //   cout << "ERROR\n";
    //   exit(-1);
    // }
    else
      algoFunc(copy, graph);

    db = traceInfo.getDB();
    res = db.insert(path1, path2, -1);
    if (2 == res)
      return 2;
    traceInfo.setDB(db);
    // traceInfo.addCmpVal(-1);
    // traceInfo.setTheLess(path1);
    // traceInfo.setTheGreater(path2);

    //   // traceInfo.getDB().insert(path1, path2, -1);
    return -1;
  }
}
