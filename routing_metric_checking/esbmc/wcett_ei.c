/* Use cbmc
 * Use MiniSAT as solver
 * Support float operation
 * Must set upper bound for each array
 * Slow
 */
#include <stdio.h>
#include <assert.h>

#define MAX 3
#define beta 50000

//int channel[MAX];

int foo(int ETT[MAX], int x[MAX]) {
  int i;
  int max = 0;
  int ret = 0, ret2 = 0;
  int channel[MAX] = {0};
  for (i = 0; i < MAX; ++i) {
    ++channel[x[i]];
    ret += ETT[i];
  }
  for (i = 0; i < MAX; ++i) {
    if (channel[i] > max)
      max = channel[i];
  }
  ret2 = (100000 - beta) * ret + beta * max;
  return ret2;
}

int bar(int a_ETT[MAX], int a_x[MAX], int b_ETT[MAX], int b_x[MAX]) {
  int i;
  int max = 0;//max_a = 0, max_b = 0;
  int ret = 0, ret2 = 0, ret3 = 0;
  int channel[MAX] = {0};
  for (i = 0; i < MAX; ++i) {
    ret += a_ETT[i];
    ret2 += b_ETT[i];
    ++channel[a_x[i]];
    ++channel[b_x[i]];
  }
  for (i = 0; i < MAX; ++i) {
    if (channel[i] > max)
      max = channel[i];
    //    if (y[i][1] > max_y)
    //      max_y = y[i][1];
  }
  ret3 = (100000 - beta) * (ret + ret2) + beta * max;//((max_x > max_y) ? max_x : max_y);
  return ret3;
}

int main() {
  int a_ETT[MAX], b_ETT[MAX], c_ETT[MAX];
  int a_x[MAX], b_x[MAX], c_x[MAX];

  int i;
  //  int sum_a = 0, sum_b = 0, sum_c = 0;
      
  for (i = 0; i < MAX; i++) {
    __ESBMC_assume(0 < a_ETT[i] && a_ETT[i] < 100
		   && 0 <= a_x[i] && a_x[i] < MAX
		   && 0 < b_ETT[i] && b_ETT[i] < 100
		   && 0 <= b_x[i] && b_x[i] < MAX
		   && 0 < c_ETT[i] && c_ETT[i] < 100
		   && 0 <= c_x[i] && c_x[i] < MAX);
    //		     && b[i][0] < 100 && b[i][0] > 0 && b[i][1] <= MAX
    //		     && c[i][0] < 100 && c[i][0] > 0 && c[i][1] <= MAX);
    /*    sum_a += a[i][1];
	  sum_b += b[i][1];
	  sum_c += c[i][1];*/
  }
  //  __ESBMC_assume(sum_a > 0 && sum_b > 0 && sum_c > 0);
  
  int res0, res1, res2, res3, res4, res5;

  res0 = foo(a_ETT, a_x);
  res1 = foo(b_ETT, b_x);
  res2 = bar(a_ETT, a_x, c_ETT, c_x);
  res3 = bar(b_ETT, b_x, c_ETT, c_x);
  /* res4 = bar(c_ETT, c_x, a_ETT, a_x); */
  /* res5 = bar(c_ETT, c_x, b_ETT, b_x); */
  
  __ESBMC_assume(res0 > res1);

  // left-isotonicity check (false)
  /* assert(res4 >= res5); */

  // right-isotonicity check (false)
  assert(res2 >= res3);


  // left-monotonicity check (true)
  /* assert(res4 >= res0); */

  // right-monotonicity check (true)
  /* assert(res2 >= res0); */


  return 0;
}
