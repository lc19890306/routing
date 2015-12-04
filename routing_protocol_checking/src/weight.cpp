#include "weight.h"

unsigned weight(const Path<int> &path) {
  unsigned s = path.head();
  unsigned t = path.rear();

  int w = 0;
  int i = 0;
  int size = 0;

  if (path.empty())
    w = Graph::matrix[s][t];
  else {
    size = path.size();
    for (i = 0; i < size - 1; ++i) {
      w += Graph::matrix[path[i]][path[i+1]];
    }
  }

  return w;
}


unsigned weight(std::vector<int> path[maxnum][maxnum], unsigned s, unsigned t1, unsigned t2) {

  int w = 0;
  int i = 0;
  int size = 0;
  if (0 == t2) {
    if (path[s][t1].empty())
      w = Graph::matrix[s][t1];
    else {
      size = path[s][t1].size();
      for (i = 0; i < size - 1; ++i)
	w += Graph::matrix[path[s][t1][i]][path[s][t1][i+1]];
    }
  }
  else {
    size = path[s][t1].size();
    for (i = 0; i < size - 1; ++i)
      w += Graph::matrix[path[s][t1][i]][path[s][t1][i+1]];
    size = path[t1][t2].size();
    for (i = 0; i < size - 1; ++i)
      w += Graph::matrix[path[t1][t2][i]][path[t1][t2][i+1]];
  }
  return w;
}
