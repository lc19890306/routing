#ifndef COUNTERS_HH
#define COUNTERS_HH

// #include <ck_pr.h>
#include <atomic>

class Counters {
public:
  static std::atomic<uint64_t> query_cnt;
  static std::atomic<uint64_t> insert_cnt;
};

#endif	// COUNTERS_HH
