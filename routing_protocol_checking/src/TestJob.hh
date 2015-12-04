#ifndef TESTJOB_HH
#define TESTJOB_HH

#include "TThreadPool.hh"

#include <unistd.h>
#include <iostream>

class TestJob : public ThreadPool::TPool::TJob {
public:
  TestJob(const int &i = -1);

  virtual void run(void *);
};

#endif	// TESTJOB_HH
