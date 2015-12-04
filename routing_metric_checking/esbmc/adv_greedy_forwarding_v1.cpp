/* Original Greedy Forwarding Metric
 * suppose q_xy == q_yx == q
 * lacking in Odd symmetry
 */

#include <stdio.h>
#include "cmath"

#define PHI 5

float dist(float x[], float d[]) {
  float res = (x[0]-d[0]) * (x[0]-d[0]) + (x[1]-d[1]) * (x[1]-d[1]);
  __ESBMC_assume(res >= 0);
  float ret = sqrt(res);
  return ret;
}

int main() {
  float x[2], y[2], d[2], q;
  float dist_xd, dist_yd, dist_xy;
  float weight_xy, weight_yx;

  dist_xd = dist(x, d);
  dist_yd = dist(y, d);
  dist_xy = dist(x, y);
  __ESBMC_assume(dist_xd != 0
		   && dist_yd != 0
		   && dist_xy != 0);
  __ESBMC_assume(0 <= x[0] && x[0] < 100
		   && 0 <= x[1] && x[1] < 100
		   && 0 <= y[0] && y[0] < 100
		   && 0 <= y[1] && y[1] < 100
		   && 0 <= d[0] && d[0] < 100
		   && 0 <= d[1] && d[1] < 100
		   && 0 < q && q < 1000000);

  weight_xy = dist_xd - dist_yd + q;
  weight_yx = dist_yd - dist_xd + q;

  //Odd_symmetry
  __ESBMC_assume(weight_xy < PHI);
  __ESBMC_assert(weight_yx > PHI, "Odd_symmetry doesn't hold!");

  return 0;
}


/* 

#include <stdio.h>
#include "CarmSqrt.h"

#define PHI 5

float dist(float x[], float d[]) {
  float res = (x[0]-d[0]) * (x[0]-d[0]) + (x[1]-d[1]) * (x[1]-d[1]);
  float ret = CarmSqrt(res);
  return ret;
}

float w(float dist_xd, float dist_yd, float q) {
  float ret;
  ret = dist_xd - dist_yd + q;
  return ret;
}

int main() {
  float x[2], y[2], d[2], q;
  float dist_xd = dist(x, d);
  float dist_yd = dist(y, d);
  float dist_xy = dist(x, y);

  __ESBMC_assume((int)dist_xd != 0
		   && (int)dist_yd != 0
		   && (int)dist_xy != 0);

  __ESBMC_assume(0 <= x[0] && x[0] < 100
		   && 0 <= x[1] && x[1] < 100
		   && 0 <= y[0] && y[0] < 100
		   && 0 <= y[1] && y[1] < 100
		   && 0 <= d[0] && d[0] < 100
		   && 0 <= d[1] && d[1] < 100);

  float w_xy = w(dist_xd, dist_yd, q);
  float w_yx = w(dist_yd, dist_xd, q);

  // Odd symmetry test
  __ESBMC_assume(w_xy < PHI);
  assert(w_yx > PHI);

  return 0;
}
*/
