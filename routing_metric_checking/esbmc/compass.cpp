#include <stdio.h>
#include "cmath"

#define PHI 0
#define MAX 3

float dist(int u[], int d[]) {
  float res = (u[0]-d[0]) * (u[0]-d[0]) + (u[1]-d[1]) * (u[1]-d[1]);
  __ESBMC_assume(res > 0);
  float ret = sqrt(res);//CarmSqrt(res);
  __ESBMC_assume(ret > 0);

  return ret;
}


float weight(int u[], int v[], int d[]) {
  __ESBMC_assume(u[0] != v[0] || u[1] != v[1]
		   && u[0] != d[0] || u[1] != d[1]);
  int prod = (v[0]-u[0])*(d[0]-u[0])+(v[1]-u[1])*(d[1]-u[1]);
  float dist_uv_sq = dist(u, v); //(u[0]-v[0]) * (u[0]-v[0]) + (u[1]-v[1]) * (u[1]-v[1]); // should be dist in reality
  float dist_ud_sq = dist(u, d); //(u[0]-d[0]) * (u[0]-d[0]) + (u[1]-d[1]) * (u[1]-d[1]); // should be dist in reality
  return -prod/(dist_uv_sq*dist_ud_sq);
}

int main() {
  int u[2], v[2], d[2];

  __ESBMC_assume(0 <= u[0] && u[0] < 100
		 && 0 <= u[1] && u[1] < 100
		 && 0 <= v[0] && v[0] < 100
		 && 0 <= v[1] && v[1] < 100
		 && 0 <= d[0] && d[0] < 100
		 && 0 <= d[1] && d[1] < 100);

  float w_uv = weight(u, v, d);
  // float w_vu = weight(v, u, d);
  
  // Odd symmetry test
  // __ESBMC_assume(w_uv <= PHI);
  // __ESBMC_assert(w_vu >= PHI, "Odd symmetry violated!");

  /* p[i][0] --> x coordinate of point
   * p[i][1] --> y coordinate of point
   */
  int i;
  int p[MAX][2];

  for (i = 0; i < MAX; ++i) {
    __ESBMC_assume(0 <= p[i][0] && p[i][0] < 100
  		     && 0 <= p[i][1] && p[i][1] < 100);
  }

  // Transitivity
  for (i = 0; i < MAX-1; ++i) {
    __ESBMC_assume(weight(p[i], p[i+1], d) <= PHI);
  }
  __ESBMC_assert(weight(p[0], p[MAX-1], d) <= PHI, "Transitivity violated!");
  
  // Strict order
  // int w[2];

  // __ESBMC_assume(0 <= w[0] && w[0] < 100
  // 		   && 0 <= w[1] && w[1] < 100);

  // __ESBMC_assume(u[0] != w[0] || u[1] != w[1]);

  // float w_uw = weight(u, w, d);

  // __ESBMC_assume(v[0] != w[0] || v[1] != w[1]);
  // __ESBMC_assert(w_uv != w_uw, "Strict order violated!");

  return 0;
}
