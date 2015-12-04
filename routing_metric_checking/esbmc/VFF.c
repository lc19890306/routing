/* VFF - Virtual Force Forwarding
 * a routing protocol may want to avoid routing its packet through /
 * a compromised zone
 * It has odd symmetry, transitivity, source independence and local minimum freeness properties
 * lacks strict order property
 */

#include <stdio.h>
//#include "CarmSqrt.h"

#define PHI 0
#define MAX 3
#define ALPHA 0.5

float weight(int u[], int v[], int h[], int d[]) {
  __ESBMC_assume(0 <= u[0] && u[0] < 100
		   && 0 <= u[1] && u[1] < 100
		   && 0 <= v[0] && v[0] < 100
		   && 0 <= v[1] && v[1] < 100
		   && 0 <= h[0] && h[0] < 100
		   && 0 <= h[1] && h[1] < 100
		   && 0 <= d[0] && d[0] < 100
		   && 0 <= d[1] && d[1] < 100);

  /* __ESBMC_assume((u[0] != v[0] || u[1] != v[1]) */
  /* 		   && (u[0] != h[0] || u[1] != h[1]) */
  /* 		   && (v[0] != h[0] || v[1] != h[1]) */
  /* 		   && (u[0] != d[0] || u[1] != d[1]) */
  /* 		   && (v[0] != d[0] || v[1] != d[1]) */
  /* 		   && (h[0] != d[0] || h[1] != d[1])); */
  __ESBMC_assume(((u[0]-d[0]) * (u[0]-d[0]) + (u[1]-d[1]) * (u[1]-d[1])) > 0
		 && ((u[0]-h[0]) * (u[0]-h[0]) + (u[1]-h[1]) * (u[1]-h[1])) > 0
		 && ((v[0]-d[0]) * (v[0]-d[0]) + (v[1]-d[1]) * (v[1]-d[1])) > 0
		 && ((v[0]-h[0]) * (v[0]-h[0]) + (v[1]-h[1]) * (v[1]-h[1])) > 0);
  float part1 = ALPHA    / ((u[0]-d[0]) * (u[0]-d[0]) + (u[1]-d[1]) * (u[1]-d[1]));
  float part2 = (1-ALPHA)/ ((u[0]-h[0]) * (u[0]-h[0]) + (u[1]-h[1]) * (u[1]-h[1]));
  float part3 = ALPHA    / ((v[0]-d[0]) * (v[0]-d[0]) + (v[1]-d[1]) * (v[1]-d[1]));
  float part4 = (1-ALPHA)/ ((v[0]-h[0]) * (v[0]-h[0]) + (v[1]-h[1]) * (v[1]-h[1]));
  __ESBMC_assume(part1 > 0 && part2 > 0 && part3 > 0 && part4 > 0);
  float ret = (part1-part2)-(part3-part4);
  return ret;
}

int main() {
  int u[2], v[2], h[2], d[2];



  float w_uv = weight(u, v, h, d);
  float w_vu = weight(v, u, h, d);
  
  // Odd symmetry test
  //  __ESBMC_assume(w_uv <= PHI);
  //  __ESBMC_assert(w_vu >= PHI, "Odd symmetry violated!");
  
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
    __ESBMC_assume(weight(p[i], p[i+1], h, d) <= PHI);
  }
  __ESBMC_assert(weight(p[0], p[MAX-1], h, d) <= PHI, "Transitivity violated!");
  /*  
  // Strict order
  float w[2];

  __ESBMC_assume(0 <= w[0] && w[0] < 100
		   && 0 <= w[1] && w[1] < 100);

  float w_uw = weight(u, w, h, d);

  __ESBMC_assume(v[0] != w[0] || v[1] != w[1]);
  __ESBMC_assert(w_uv != w_uw, "Strict order violated!");
     */
  return 0;
}
