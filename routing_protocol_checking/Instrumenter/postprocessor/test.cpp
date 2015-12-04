#include <iostream>

using namespace std;

void f(int &n) {
  static bool flag = true;
  if (flag) {
    n = 5;
    flag = false;
  }
}

int main(int argc, char **argv) {
  // int x(0);
  // {
  //   int x(1);
  //   cout << x << endl;
  // }
  //   cout << x << endl;
  int x = 10;
  bool flag = true;
  if (argc > 1)
    goto L2;
 L1:
  int n;
  if (!flag)
    goto L3;
  // cout << n << endl;
  while (x-- > 0) {
    ++n;
  L2:
    if (flag) {
      flag = false;
      goto L1;
    }
  L3:
    f(n);
    flag = false;
    ++n;
    cout << n << endl;
  }
  return 0;
}
