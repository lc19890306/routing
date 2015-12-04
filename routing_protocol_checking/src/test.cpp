// #include "TestJob.hh"

// #include <iostream>
// #include <pthread.h>
// #include <cstring>
// #include <vector>
// // #include <unordered_set>
// #include <set>
// #include <bitset>
// #include <algorithm>
// #include <deque>
// #include <map>
// #include <cstdlib>
// #include <cstring>
// #include <memory>

// using namespace std;

// // class A {
// // public:
// //   A(int &n) : _n(&n) {*_n = 5;}
// //   int *_n;
// // };

// // struct block {
// //   template<int input>
// //   void func() {
// //     cout << input << endl;
// //   }
// // };

// // void func(const bool s[10][10], bool t[10][10]) {
// //   memcpy(t, s, sizeof(bool) * 10 * 10);
// // }

// // int main() {
// //   // int i;
// //   // A a(i);
// //   // cout << i << endl;
// //   // cout << 2 + (3 >> 1) << endl;
// //   // cout << (68.5 > 68) << endl;
// //   // std::deque<int> q(2);
// //   // q.front() = 0;
// //   // q.back() = 1;
// //   // cout << q.front() << q.back() << endl;
// //   // q.pop_front();
// //   // cout << q.front() << q.back() << endl;
// //   // q.back() = 2;
// //   // cout << q.front() << q.back() << endl;
// //   // std::deque<int> empty_queue;
// //   // q.swap(empty_queue);
// //   // cout << q.size() << endl;
// //   block *b;
// //   b = new block;
// //   b->func<3>();

// //   bool s[10][10] = {{0}, {0}};
// //   bool t[10][10];
// //   s[5][7] = 1;
// //   func(s, t);
// //   for (int i = 0; i < 10; ++i) {
// //     for (int j = 0; j < 10; ++j)
// //       cout << t[i][j] << " ";
// //     cout << endl;
// //   }

// //   return 0;
// // }

// class Base {
// public:
//   Base() : pi(NULL) {}
//   Base(const Base &base) : pi(base.pi) {}
//   ~Base() {
//     if (NULL != pi) {
//       delete pi;
//       pi = NULL;
//     }
//   }
//   void setPI(int * const &pi) {
//     this->pi = pi;
//   }
//   int * getPI() const {
//     return pi;
//   }

// private:
//   int *pi;
// };

// class Derived1 : public Base {
// public:
//   int a;
// };

// class Derived2 : public Base {
// public:
//   double b;
// };

// int main() {
//   // std::vector<Base *> v;
//   // Derived1 d1;
//   // Derived2 d2;
//   // v.push_back(&d1);
//   // v.push_back(&d2);
//   // static_cast<Derived1 *>(v[0])->a = 1;
//   // static_cast<Derived2 *>(v[1])->b = 3.14;

//   // cout << static_cast<Derived1 *>(v[0])->a << endl;
//   // cout << static_cast<Derived2 *>(v[1])->b << endl;
//   // unsigned n;
//   // cin >> n;

//   // switch (n) {
//   // case 5:
//   // case 7:
//   //   cout << n << endl;
//   // default:
//   //   break;
//   // }

//   // Base a;
//   // Base b(a);

//   // cout << a.getPI() << " " << b.getPI() << endl;
//   // b.setPI(new int(5));
//   // cout << a.getPI() << " " << b.getPI() << endl;

//   // const int x = 1;
//   // int y = x;

//   // ThreadPool::init(32);

//   // std::size_t x(100000);

//   // while (x-- > 0) {
//   //   TestJob *job = new TestJob();
//   //   ThreadPool::run(job, NULL, true);
//   // }

//   // ThreadPool::sync_all();
//   // ThreadPool::done();

//   std::vector<std::set<std::string> > vsstr;// = {"1-2-3", "1-3-4-2", "1-5-3-2"};
//   // std::map<std::string, const void *> m;
//   // std::string str("1-2-3");
//   // m.insert(std::make_pair(str, str.c_str()));
//   // std::cout << static_cast<const void *>(str.c_str()) << std::endl;
//   // std::cout << static_cast<const void *>(m[str]) << std::endl;

//   // std::cout << static_cast<const void *>(sstr.find("1-2-3")) << std::endl;
//   // char buffer[6];
//   // std::vector<std::set<std::string *> > vspstr;
//   std::vector<std::set<std::shared_ptr<std::string> > > vspstr;

//   std::size_t n(3);
//   while (n-- != 0) {
//     // std::set<std::shared_ptr<std::string> > spstr;
//     // std::set<std::string *> spstr;
//     std::set<std::string> sstr;
//     for (std::size_t i = 0; i != 10000000; ++i) {
//       std::shared_ptr<std::string> pstr(new std::string(std::to_string(i)));
//       sstr.insert(*pstr);
//       // spstr.insert(pstr);
//     }
//     // vspstr.push_back(spstr);
//     vsstr.push_back(sstr);
//   }

//   return 0;
// }

#include "relational_db.h"

int main() {
  rDB rdb;
  rdb.insert("2-1", "2-3", -1);
  rdb.insert("2-1-3", "2-3", -1);
  rdb.insert("2-1-3", "2-1-5", -1);
  rdb.insert("2-1-3-4", "2-1-5", -1);
  rdb.insert("2-1-3-4-6", "2-1-5", -1);
  rdb.insert("2-1-5", "2-1-3-4-6-5", -1);
  std::cout << rdb.query("2-1-3-4-6", "2-3-1-5-4-6") << std::endl;
  rdb.print();
  return 0;
}
