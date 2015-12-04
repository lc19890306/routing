/* LPF - Line Progress Forwarding
 * A packet is forwarded to the closest node to the destination /
 * among all the neighbors of the forwarding node that are closer /
 * to the destination than the forwarding node
 * It has odd symmetry, transitivity, and local minimum freeness properties
 * lacks strict order and source independence properties
 */

#include <stdio.h>
#include "cmath"

#define PHI 0
#define MAX 3

float dist(int u[], int d[]) {
  float res = (u[0]-d[0]) * (u[0]-d[0]) + (u[1]-d[1]) * (u[1]-d[1]);
  __ESBMC_assume(res > 0); // res < 0 if no assumption
  float ret = sqrt(res);
  __ESBMC_assume(ret > 0); // the result of sqrt (from cmath) is less that 0 if no assumption

  return ret;
}

float weight(int u[], int v[], int s[], int d[]) {
  int prod = (d[0]-s[0])*(v[0]-u[0])+(d[1]-s[1])*(v[1]-u[1]);
  __ESBMC_assume(u[0] != v[0] || u[1] != v[1]);
  float dist_sd = dist(s, d);
  __ESBMC_assume(dist_sd > 0); // dist_ad may be equal to 0 if no assuption
  //  __ESBMC_assume(s[0] != d[0] || s[1] != d[1]);
  //  __ESBMC_assume(s[0] != u[0] || s[1] != u[1]);
  float ret = -prod/dist_sd;//LC_div(prod, dist_sd);
  /*  if ((10 * ret +5) * dist_sd <= 10 * prod)
    return ret - 1;
    else*/
  return ret;
}

int main() {
  int u[2], v[2], s[2], d[2];

  __ESBMC_assume(0 <= u[0] && u[0] < 100
		   && 0 <= u[1] && u[1] < 100
		   && 0 <= v[0] && v[0] < 100
		   && 0 <= v[1] && v[1] < 100
		   && 0 <= s[0] && s[0] < 100
		   && 0 <= s[1] && s[1] < 100
		   && 0 <= d[0] && d[0] < 100
		   && 0 <= d[1] && d[1] < 100);
  
  float w_uv = weight(u, v, s, d);
  float w_vu = weight(v, u, s, d);
  
  // Odd symmetry test
  ///  __ESBMC_assume(w_uv <= PHI);
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
    __ESBMC_assume(weight(p[i], p[i+1], s, d) <= PHI);
  }
  __ESBMC_assert(weight(p[0], p[MAX-1], s, d) <= PHI, "Transitivity violated!");
  /*  
  // Strict order
  int w[2];

  __ESBMC_assume(0 <= w[0] && w[0] < 100
		   && 0 <= w[1] && w[1] < 100);

  float w_uw = weight(u, w, s, d);

  //  float dist_vw = dist(v, w);
  //  __ESBMC_assume((int)dist_vw != 0);
  __ESBMC_assume(v[0] != w[0] || v[1] != w[1]);

  __ESBMC_assert(w_uv != w_uw, "Strict order violated!");

  // Source independence
  int s2[2];
  __ESBMC_assume(s[0] != s2[0] || s[1] != s2[1]);
  __ESBMC_assume(0 <= s2[0] && s2[0] < 100
		   && 0 <= s2[1] && s2[1] < 100);
  float w_uv2 = weight(u, v, s2, d);
  __ESBMC_assert(w_uv == w_uv2, "Source independence violated!");
     */
  return 0;
}
