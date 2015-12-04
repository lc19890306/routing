/* RPF - Random Progress Forwarding
 * a packet is forwarded to the neighbor that has the maximum positive projected progress /
 * on the straight line segment joining the forwarding node and the destination
 * It has local minumum-free property
 * lacks odd symmetry
 */

#include <stdio.h>
#include "cmath"

#define PHI 0

float dist(int u[], int d[]) {
  float res = (u[0]-d[0]) * (u[0]-d[0]) + (u[1]-d[1]) * (u[1]-d[1]);
  __ESBMC_assume(res > 0);
  float ret = sqrt(res);//CarmSqrt(res);
  __ESBMC_assume(ret > 0);

  return ret;
}

float weight(int u[], int v[]) {
  int d[2];
  int prod = (v[0]-u[0])*(d[0]-u[0])+(v[1]-u[1])*(d[1]-u[1]);
  float dist_ud = dist(u, d);
  __ESBMC_assume(dist_ud != 0);
  __ESBMC_assume(u[0] != d[0] || u[1] != d[1]);
  float ret = -prod/dist_ud;
  return ret;
}

int main() {
  int u[2], v[2];
  /*
    __ESBMC_assume(0 <= u[0] && u[0] < 100
		 && 0 <= u[1] && u[1] < 100
		 && 0 <= v[0] && v[0] < 100
		 && 0 <= v[1] && v[1] < 100);
		 && 0 <= d[0] && d[0] < 100
		 && 0 <= d[1] && d[1] < 100);
					     */
  float w_uv = weight(u, v);
  float w_vu = weight(v, u);

  // Odd symmetry test
  __ESBMC_assume(w_uv <= PHI);
  __ESBMC_assert(w_vu >= PHI, "Odd symmetry violated!");

  return 0;
}
