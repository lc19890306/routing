/* MFR - Most Forward within Radius
 * A packet is forwarded to the closest node to the destination /
 * among all the neighbors of the forwarding node that are closer /
 * to the destination than the forwarding node
 * It has odd symmetry, transitivity, source independence, and local minimum freeness properties
 * lacks strict order
 */

#include <stdio.h>
#include "cmath"
//#include "CarmSqrt.h"

#define PHI 0
#define MAX 4

float dist(int u[], int d[]) {
  float res = (u[0]-d[0]) * (u[0]-d[0]) + (u[1]-d[1]) * (u[1]-d[1]);
  __ESBMC_assume(res >= 0);
  float ret = sqrt(res);//CarmSqrt(res);

  return ret;
}

/*
int dist_sq(int u[], int d[]) {
  int ret = (u[0]-d[0]) * (u[0]-d[0]) + (u[1]-d[1]) * (u[1]-d[1]);
  return ret;
}
*/
float weight(float dist_ud, float dist_vd) {
  float ret = dist_vd - dist_ud;
  return ret;
}

int main() {
  int u[2], v[2], d[2];

  //  int dist_uv_sq = dist_sq(u, v);

  __ESBMC_assume(u[0] != v[0] || u[1] != v[1]);

  __ESBMC_assume(0 <= u[0] && u[0] < 100
		   && 0 <= u[1] && u[1] < 100
		   && 0 <= v[0] && v[0] < 100
		   && 0 <= v[1] && v[1] < 100
		   && 0 <= d[0] && d[0] < 100
		   && 0 <= d[1] && d[1] < 100);

  //  __ESBMC_assume(dist_xd != dist_yd);

  float dist_ud = dist(u, d);
  float dist_vd = dist(v, d);

  __ESBMC_assume(dist_ud > 0
		 && dist_vd > 0);

  float w_uv = weight(dist_ud, dist_vd);
  float w_vu = weight(dist_vd, dist_ud);
    
  // Odd symmetry test
  //  __ESBMC_assume(w_uv <= PHI);
  //  assert(w_vu >= PHI);

  /* p[i][0] --> x coordinate of point
   * p[i][1] --> y coordinate of point
   *//*
  int i;
  int p[MAX][2], link[MAX];

  for (i = 0; i < MAX; ++i) {
    __ESBMC_assume(0 <= p[i][0] && p[i][0] < 100
		     && 0 <= p[i][1] && p[i][1] < 100);
    link[i] = dist(p[i], d);
    __ESBMC_assume(link[i] > 0);
    if (i < MAX-1)
      __ESBMC_assume(dist(p[i], p[i+1]) > 0);
  }

  // Transitivity
  for (i = 0; i < MAX-1; ++i) {
    __ESBMC_assume(weight(link[i], link[i+1]) <= PHI);
  }
  assert(weight(link[0], link[MAX-1]) <= PHI);
     */
  // Strict order
  int w[2];
  //  int dist_uw_sq = dist_sq(u, w);
  float dist_wd = dist(w, d);
  //  int dist_vw_sq = dist_sq(v, w);

  __ESBMC_assume(0 <= w[0] && w[0] < 100
		 && 0 <= w[1] && w[1] < 100);

  __ESBMC_assume(dist_wd > 0);
  __ESBMC_assume(u[0] != w[0] || u[1] != w[1]);
  
  float w_uw = weight(dist_ud, dist_wd);

  __ESBMC_assume(v[0] != w[0] || v[1] != w[1]);
  assert(w_uv != w_uw);
       
  return 0;
}
