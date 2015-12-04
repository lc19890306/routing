#include "ucg_generator.hh"

void generate(vector<vector<int> > &graph, const int &n, const int &max_deg) {
  int row, col;
  srand((unsigned)time(NULL));

  /* initialize the graph with no edges */
  for (row = 1; row <= n; ++row) {
    graph[row][0] = 0;
    for (col = 1; col <= n; ++col)
      graph[row][col] = inf;
    graph[row][row] = 0;
  }

  /* build the simplest connected undirected graph */
  graph[1][2] = 1;
  ++graph[1][0];
  graph[2][1] = 1;
  ++graph[2][0];
  for (int i = 3; i <= n; ++i) {
    int sink = rand() % i;
    if (0 == sink) {
      --i;
      continue;
      // ++sink;
    }
    if (max_deg > graph[sink][0]
	&& max_deg > graph[i][0]) {
      graph[sink][i] = 1;
      ++graph[sink][0];
      graph[i][sink] = 1;
      ++graph[i][0];
    }
    else {
      for (int j = 1; j < i; ++j) {
	if (max_deg > graph[j][0]
	    && max_deg > graph[i][0]) {
	  graph[i][j] = 1;
	  ++graph[i][0];
	  graph[j][i] = 1;
	  ++graph[j][0];
	  break;
	}
      }
    }
  }

  /* add more edges to the simplest connected undirected graph */
  for (row = 1; row <= n; ++row) {
    int times = max_deg - graph[row][0];
    for (int i = 0; i < times; ++i) {
      int sink = rand() % (n + 1);
      if (0 == sink) {
	--i;
	continue;
	// ++sink;
      }
      if ((1 != graph[row][sink])
	  && (max_deg > graph[sink][0])
	  && (max_deg > graph[row][0])) {
	graph[row][sink] = 1;
	++graph[row][0];
	graph[sink][row] = 1;
	++graph[sink][0];
      }
    }
    if (0 != graph[row][row]) {
      graph[row][row] = 0;
      --graph[row][0];
    }
  }
}
