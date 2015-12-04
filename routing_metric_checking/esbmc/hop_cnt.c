#include <stdio.h>

#define MAX 20

float weight(float hop) {
  return hop;
}

int main() {
  float a, b, c;

  __ESBMC_assume(0 < a && a <= 100
		 && 0 < b && b <= 100
		 && 0 < c && c <= 100);
  
  /* __ESBMC_assume(weight(a) >= weight(b)); */

  // right-isotonicity
  /* __ESBMC_assert(weight(a+c) >= weight(b+c), "Right-isotonicity violated!"); */
  
  //left-isotonicity
  /* __ESBMC_assert(weight(c+a) >= weight(c+b), "Left-isotonicity violated!"); */
  
  //right-monotonicity
  /* __ESBMC_assert(weight(a) <= weight(a+b), "Right-monotonicity violated!"); */
  
  //left-monotonicity
  __ESBMC_assert(weight(a) <= weight(b+a), "Left-monotonicity violated!");
  
  return 0;
}
