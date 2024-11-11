#ifndef __JOB_SYSTEM__
#define __JOB_SYSTEM__ 1

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class JobSystem {
public:
  JobSystem(unsigned int numThreads);
  ~JobSystem();
  JobSystem(const JobSystem& other);
  JobSystem(JobSystem& other);
  //JobSystem operator=(JobSystem& other);
  void addJob(std::function<void()> f);
  void runTasks();
  std::vector<std::thread> workers;

private:
  std::queue<std::function<void()>> tasks;
  std::mutex mutex_;
  std::atomic<bool> quit_;
  std::condition_variable condition;
};

#endif // !#define __JOB_SYSTEM__ 1
