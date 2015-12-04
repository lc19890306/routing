#include "chkpt.hh"
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


for(i=1; i<=n; ++i) {
if (v == i)
continue;

L0:
checkpoint0(path[v][i]);
if (0 != compare(inf, graph->weight(path[v][i]))) {
L1:
checkpoint1(path[v][i]);
path[v][i].push_back(v);
L2:
checkpoint2(path[v][i]);
path[v][i].push_back(i);
}
}

s[v] = 1;
    
for(i=2; i<=n; ++i) {
u = v;

for (x = 1; x <= n; ++x) {
if (s[x])
continue;

L3:
checkpoint3(path[v][x]);
if (1 != compare(inf, graph->weight(path[v][x])))
continue;
for (y = x + 1; y <= n; ++y) {
if (s[y])
continue;

L4:
checkpoint4(path[v][y]);
if (1 != compare(inf, graph->weight(path[v][y])))
continue;

L5:
checkpoint5(path[v][x]);
L6:
checkpoint6(path[v][y]);
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
for(j=1; j<=n; ++j) {
L7:
checkpoint7(path[u][j]);
if ((!s[j]) && -1 == compare(graph->weight(path[u][j]), inf)) {
L8:
checkpoint8(path[u][j]);
path[u][j].push_back(u);
L9:
checkpoint9(path[u][j]);
path[u][j].push_back(j);
tmp.clear();
L10:
checkpoint10(path[v][u]);
L11:
checkpoint11(path[u][j]);
tmp = path[v][u] + path[u][j];
L12:
checkpoint12(path[v][j]);
if (path[v][j].is_checked())
tmp.check();

L13:
checkpoint13(path[v][j]);
res = compare(tmp, path[v][j], this_traceInfo, graph);
if (2 == res)
return;
if(-1 == res) {
L14:
checkpoint14(path[v][j]);
path[v][j].clear();
L15:
checkpoint15(path[v][j]);
path[v][j] = tmp;
}
}
}
}

if (v != n) {
this_traceInfo.setOptimalPath(assemble(path[v][n]));
res = 0;

auto size = graph->v_possible_paths[v][n].size();
rDB db = rDB();
for (i = 0; i < size; ++i) {
if (this_traceInfo.getOptimalPath() != graph->v_possible_paths[v][n][i].path) {
db = this_traceInfo.getDB();
res = db.query(this_traceInfo.getOptimalPath(), graph->v_possible_paths[v][n][i].path);
if (2 == res)
return;
this_traceInfo.setDB(db);
if (res == 0) {

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&lock);
db = this_traceInfo.getDB();
db.print();
res = db.insert(this_traceInfo.getOptimalPath(), graph->v_possible_paths[v][n][i].path, 1);
if (2 == res)
return;
this_traceInfo.setDB(db);
cout << this_traceInfo.getOptimalPath() << " -vs- " << graph->v_possible_paths[v][n][i].path << " : " << res << endl << endl;
cout << "counterexample:\n";
this_traceInfo.getDB().print();
cout << endl;
pthread_mutex_unlock(&lock);
exit(0);
return;
}
}
}
}
pthread_mutex_lock(&graph->v_lock[v]);
graph->v_traces[v].push_back(this_traceInfo);
pthread_mutex_unlock(&graph->v_lock[v]);
}











