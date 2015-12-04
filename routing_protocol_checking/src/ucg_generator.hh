#ifndef __HH_UCG_GENERATOR_
#define __HH_UCG_GENERATOR_

#include <ctime>
#include <cstdlib>
#include "headers.h"

using namespace std;

void generate(vector<vector<int> > &graph, const int &n = 5, const int &max_deg = 3);

/* void generate(int graph[maxnum][maxnum], const int &n = 5, const int &max_deg = 4) { */
/*   int row, col; */
/*   srand((unsigned)time(NULL)); */
/*   for (row = 1; row <= n; ++row) { */
/*     bool connected = false; */
/*     for (col = 1; col < row; ++col) { */
/*       /\* srand((unsigned)time(NULL)); *\/ */
/*       unsigned x = rand() % 2; */
/*       if (1 == x) { */
/* 	connected = true; */
/* 	graph[row][col] = 1; */
/*       } */
/*       else */
/* 	graph[row][col] = inf; */
/*     } */
/*     if (false == connected) { */
/*       unsigned x = rand() % row; */
/*       if (0 != x) */
/* 	graph[row][x] = 1; */
/*       else */
/* 	graph[row][1] = 1; */
/*     } */
/*     graph[row][row] = 0; */
/*   } */
/*   for (row = 1; row <= n; ++row) */
/*     for (col = row + 1; col <= n; ++col) */
/*       graph[row][col] = graph[col][row]; */
/* } */

#endif	// __HH_UCG_GENERATOR_
