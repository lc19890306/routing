#include "TestJob.hh"

TestJob::TestJob(const int &i) : ThreadPool::TPool::TJob(i) {}

void TestJob::run(void *) {
  std::cout << "test\n";
  usleep(1000);
}
