#ifndef __H_HEADERS_
#define __H_HEADERS_

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <pthread.h>
#include <atomic>
/* #include "functions.h" */

#include "AddressBook.hh"
/* #include "TraceInfoQueue.h" */

using namespace std;

#define maxnum 17
#define inf 0x3f3f3f3f
#define MAX_LEVEL 4

/* #include "Graph.hh" */

class runningThread {
 public:
  /* static pthread_mutex_t lock; */
  static std::atomic<uint64_t> counter;
  static bool enq;
};

template <typename T>
class Singleton {
 public:
  typedef T object_type;
  static T &instance() {
    static T obj;
    create_object.do_nothing();
    return obj;
  }

 private:
  struct object_creator {
    object_creator() {
      Singleton<T>::instance();
    }
    inline void do_nothing() const {}
  };

  static object_creator create_object;
};

/* #include "dijkstra.h" */
/* #include "compare.h" */

#endif	/* __H_HEADERS_ */
