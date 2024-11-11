#include "JobSystem.hpp"

JobSystem::JobSystem(unsigned int numThreads)
{
  for (size_t i = 0; i < numThreads; ++i) {
    workers.push_back(std::thread{ [this]() { runTasks(); } });
  }
}

JobSystem::~JobSystem() {
  quit_ = true;
  condition.notify_all();
  for (auto& t : workers) {
    t.join();
  }
}

JobSystem::JobSystem(const JobSystem& other) {}

void JobSystem::runTasks() {
  while (true) {
    std::function<void()> job;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      condition.wait(lock, [this]() { return quit_ || !tasks.empty(); });
      if (quit_ && tasks.empty()) return; // Salir si quit_ es true y no hay tareas

      job = std::move(tasks.front());
      tasks.pop();
    }
    job(); // Ejecutar el trabajo fuera del bloqueo
  }
}

void JobSystem::addJob(std::function<void()> f) {
  //CRITICAL SECTION START
  std::lock_guard<std::mutex> lock{ mutex_ };
  tasks.emplace(f);
  condition.notify_one();
  //CRITICAL SECTION END
}