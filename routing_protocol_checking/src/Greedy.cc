#include "Greedy.hh"

void initialize(std::vector<std::vector<Path<int> > > &path) {
  for (std::size_t i = 1; i != path.size(); ++i) {
    for (std::size_t j = 1; j != path[i].size(); ++j) {
      path[i][j].set_head(i);
      path[i][j].set_rear(j);
    }
  }
}

void
Greedy(const TraceInfo &traceInfo, Graph *graph) {
  const auto n(traceInfo.getLast());
  const auto v(traceInfo.getFirst());

  int res(0), i(0), j(0);
  TraceInfo this_traceInfo(traceInfo);
  std::vector<std::vector<Path<int> > > path(n + 1, std::vector<Path<int> >(n + 1));
  initialize(path);

  auto cur_node(v);
  while (cur_node != n) {
    auto next_node(cur_node);
    for (i = 1; i <= n; ++i) {
      if (cur_node == i)
	continue;
      if (path[cur_node][i].empty()) {
	if (compare(inf, graph->weight(path[cur_node][i]))) {
	  path[cur_node][i].push_back(cur_node);
	  path[cur_node][i].push_back(i);
	}
	else
	  continue;
      }
      for (j = i + 1; j <= n; ++j) {
	if (cur_node == j)
	  continue;
	if (path[cur_node][j].empty()) {
	  if (compare(inf, graph->weight(path[cur_node][j]))) {
	    path[cur_node][j].push_back(cur_node);
	    path[cur_node][j].push_back(j);
	  }
	  else
	    continue;
	}
	res = compare(path[cur_node][i], path[cur_node][j], this_traceInfo, graph);
	if (2 == res)
	  continue;
	if (1 == res) {
	  next_node = j;
	  i = j;
	  break;
	}
      }
      if (i != j) {
	next_node = i;
	break;
      }
    }
    path[v][next_node] = path[v][cur_node] + path[cur_node][next_node];
    if (path[v][next_node].empty()) {
      cout << "Routing loop detected!" << endl;
      cout << assemble(path[v][cur_node]) << " + " << assemble(path[cur_node][next_node]) << endl;
      this_traceInfo.getDB().print();
      exit(1);
    }
    // Directly return if greedy routing comes to a dead end
    if (cur_node == next_node)
      return;
    cur_node = next_node;
  }
  this_traceInfo.setOptimalPath(assemble(path[v][n]));
  // pthread_mutex_lock(&graph->v_lock[v]);
  // // this_traceInfo.getDB().print();
  // graph->v_traces[v].push_back(this_traceInfo);
  // pthread_mutex_unlock(&graph->v_lock[v]);
  // std::vector<std::vector<Path<int> > >().swap(path);
}
