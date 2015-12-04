/* PLQ - Enhanced Greedy Routing Metric
 * tries to capture both the progress and link quality
 * lacking in Odd symmetry
 */

#include <stdio.h>
#include "cmath"

#define PHI 0
#define MAX 3

float dist(float x[], float d[]) {
  float res = (x[0]-d[0]) * (x[0]-d[0]) + (x[1]-d[1]) * (x[1]-d[1]);
  __ESBMC_assume(res >= 0);
  float ret = sqrt(res);
  __ESBMC_assume(ret >= 0);
  return ret;
}

float w(float dist_xd, float dist_yd, float q) {
  float  g_xy, ret = 0;
  g_xy = dist_xd - dist_yd;
  if (g_xy > 0) {
    ret = g_xy + q;
    __ESBMC_assume(ret > 0);
  }
  else
    ret = 0;
  return ret;
}

int main() {
  float x[2], y[2], d[2], q;
  float dist_xd = dist(x, d);
  float dist_yd = dist(y, d);
  float dist_xy = dist(x, y);

  __ESBMC_assume(dist_xd > 0
		   && dist_yd > 0
		   && dist_xy > 0);

  __ESBMC_assume(0 <= x[0] && x[0] < 100
		   && 0 <= x[1] && x[1] < 100
		   && 0 <= y[0] && y[0] < 100
		   && 0 <= y[1] && y[1] < 100
		   && 0 <= d[0] && d[0] < 100
		   && 0 <= d[1] && d[1] < 100
		   && 0 < q && q < 1000000);

  __ESBMC_assume(dist_xd != dist_yd);
  
  float w_xy = w(dist_xd, dist_yd, q);
  float w_yx = w(dist_yd, dist_xd, q);

  // Odd symmetry test
  __ESBMC_assume(w_xy <= PHI);
  __ESBMC_assert(w_yx >= PHI, "Odd symmetry violated!");

  /* p[i][0] --> x coordinate of point
   * p[i][1] --> y coordinate of point
  // int i;
  // float p[MAX][2], link[MAX], qual[MAX];

  // for (i = 0; i < MAX; ++i) {
  //   __ESBMC_assume(0 <= p[i][0] && p[i][0] < 100
  // 		     && 0 <= p[i][1] && p[i][1] < 100
  // 		     && 0 < qual[i] && qual[i] < 1000000);

  //   link[i] = dist(p[i], d);
  //   __ESBMC_assume(link[i] != 0);
  //   if (i < MAX-1)
  //     __ESBMC_assume(dist(p[i], p[i+1]) != 0);
  // }

  // // Transitivity
  // for (i = 0; i < MAX-1; ++i) {
  //   __ESBMC_assume(w(link[i], link[i+1], qual[i]) <= PHI);
  // }

  // __ESBMC_assert(w(link[0], link[MAX-1], qual[MAX-1]) <= PHI, "Transitivity violated!");
  */
  return 0;
}
