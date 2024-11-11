#ifndef __JOB_SYSTEM__
#define __JOB_SYSTEM__ 1

#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class JobSystem {
public:
  enum JobType {
    Render,
    Physics,
    AI,
    Audio,
    General
  };
  enum JobPriority {
    Low,
    Medium,
    High
  };
  struct PrioritizedJob {
    JobPriority priority;
    std::function<void()> job;

    // Operador de comparación para priorizar trabajos en la priority_queue
    bool operator<(const PrioritizedJob& other) const {
      return static_cast<int>(priority) < static_cast<int>(other.priority);
    }
  };
  JobSystem(unsigned int numThreads);
  ~JobSystem();
  JobSystem(const JobSystem& other);
  JobSystem(JobSystem& other);
  //JobSystem operator=(JobSystem& other);
  void addJob(JobType type, JobPriority priority, std::function<void()> f);
  void runTasks();
  std::vector<std::thread> workers;

private:
  std::unordered_map<JobType, std::priority_queue<PrioritizedJob>> tasks;
  std::mutex mutex;
  std::atomic<bool> quit;
  std::condition_variable condition;
};

#endif // !#define __JOB_SYSTEM__ 1
