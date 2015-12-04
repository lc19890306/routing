#include "bellman_ford.hh"
#include "checkpoint.hh"

void initialize(std::vector<std::vector<Path<int> > > &path) {
  for (std::size_t i = 1; i != path.size(); ++i) {
    for (std::size_t j = 1; j != path[i].size(); ++j) {
      path[i][j].set_head(i);
      path[i][j].set_rear(j);
    }
  }
}

void Bellman_Ford(const TraceInfo &traceInfo, Graph *graph) {
  const auto n = traceInfo.getLast();
  const auto v = traceInfo.getFirst();

  int res(0);
  TraceInfo this_traceInfo(traceInfo);
  std::vector<std::vector<Path<int> > > path(n + 1, std::vector<Path<int> >(n + 1));
  initialize(path);
  bool relaxed;
  std::size_t i, row, col;
  Path<int> tmp;

  switch (this_traceInfo.getLabel()) {
  case 1:
    goto L1;
  case 2:
    goto L2;
  case 3:
    goto L3;
  case 4:
    goto L4;
  default:
    break;
  }

  for(i=1; i<=n; ++i) {
    // s[i] = 0;     // 初始都未用过该点
    if (v == i)
      continue;

  L1:
    // checkpoint(1, path[v][i], graph, n, v, res, this_traceInfo, i, j, path, s, u, x, y, tmp);
    if (0 != compare(inf, graph->weight(path[v][i]))) {
      /* if (0 != compare(inf, weight(c, path, v, i))) { */
      path[v][i].push_back(v);
      path[v][i].push_back(i);
    }
  }

  relaxed = true;
  for(i = 1; i <= n - 1; ++i) {
    if (relaxed)
      relaxed = false;
    else
      break;
    for (row = 1; row <= n; ++row) {
      if (v == row)
	continue;
      for (col = 1; col <= n; ++col) {
      L2:
	if (0 >= compare(inf, graph->weight(path[row][col]))
	    || 0 >= compare(inf, graph->weight(path[v][row])))
	// if (c[row][col] >= inf)
	  continue;
	if (row == col)
	  continue;
	if (path[row][col].empty()) {
	  path[row][col].push_back(row);
	  path[row][col].push_back(col);
	}
	// if (path[v][row].empty()) {
	//   path[v][row].push_back(v);
	//   path[v][row].push_back(row);
	// }
      L3:
	tmp = path[v][row] + path[row][col];
	if (tmp.empty())
	  continue;

      L4:
	checkpoint(4, path[v][col], graph, n, v, res, this_traceInfo, i, row, col, path, tmp);
	res = compare(path[v][col], tmp, this_traceInfo, graph);
	if (2 == res)
	  return;
	if (1 == res) {
	// if(c[original][col] > c[original][row] + c[row][col]) {//松弛（顺序一定不能反~）  
	  path[v][col].clear();
	  path[v][col] = tmp;
	  // c[original][col] = c[original][row] + c[row][col];
	  // pre[col] = row;  
	  relaxed = true;
	}  
      }
    }
  }

  checkOptimality(graph, this_traceInfo, path[v][n]);
  // if (v != n) {
  //   pthread_mutex_lock(&graph->v_lock[v]);
  //   // this_traceInfo.getDB().print();
  //   graph->v_traces[v].push_back(this_traceInfo);
  //   pthread_mutex_unlock(&graph->v_lock[v]);
  // }
  std::vector<std::vector<Path<int> > >().swap(path);
}
