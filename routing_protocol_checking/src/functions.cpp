#include "functions.h"
#include <iostream>
using namespace std;

string itoa(const int &n) {
  std::stringstream ss;
  ss << n;
  string str(ss.str());
  return str;
}

// string itoa(const int &n) {
//   strstream ss;
//   string str;
//   ss << n;
//   ss >> str;
//   return str;
// }

// string itoa(const int &n) {
//   std::string str = std::to_string(n);
//   return str;
// }

// string assemble(const Path<int> &path) {
//   // static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//   // pthread_mutex_lock(&lock);

//   int i, size = path.size();
//   string res;
//   for (i = 0; i < size - 1; ++i) {
//     res.append(itoa(path[i]));
//     res.append("-");
//   }
//   res.append(itoa(path[i]));

//   // pthread_mutex_unlock(&lock);

//   return res;
// }

String assemble(const Path<int> &path) {
  // static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  // pthread_mutex_lock(&lock);

  String ret;
  for (auto i(0); i != path.size(); ++i)
    ret.push_back(path[i]);

  return ret;
}
