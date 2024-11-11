#include "JobSystem.hpp"
#include <iostream>

JobSystem::JobSystem(unsigned int numThreads)
{
  for (size_t i = 0; i < numThreads; ++i) {
    workers.push_back(std::thread{ [this]() { runTasks(); } });
  }
}

JobSystem::~JobSystem() {
  quit = true;
  condition.notify_all();
  for (auto& t : workers) {
    t.join();
  }
}

JobSystem::JobSystem(const JobSystem& other) {}
/*
void JobSystem::runTasks() {
  PrioritizedJob job;
  while (true) {
    JobType selectedType;
    bool jobFound = false;
    {
      std::unique_lock<std::mutex> lock(mutex);
      condition.wait(lock, [this]() {
        if (quit) return true;
        for (const auto& [type, queue] : tasks) {
          if (!queue.empty()) return true;
        }
        return false;
        });
      if (quit && tasks.empty()) return; // Salir si quit_ es true y no hay tareas

      // Buscar el trabajo de mayor prioridad entre todas las colas
      job.priority = JobPriority::Low;
      for (auto& [type, queue] : tasks) {
        if (!queue.empty() && queue.top().priority >= job.priority) {
          job.priority = queue.top().priority;
          selectedType = type;
          job.job = queue.top().job;
          jobFound = true;
        }
      }

      // Si encontramos un trabajo, lo removemos de su cola
      if (jobFound) {
        tasks[selectedType].pop();
      }
    }
  }
  switch (job.priority)
  {
  case JobSystem::High:
    std::cout << "JobFound: Priority High\n" << std::endl;
    break;
  case JobSystem::Medium:
    std::cout << "JobFound: Priority Medium\n" << std::endl;
    break;
  case JobSystem::Low:
    std::cout << "JobFound: Priority Low\n" << std::endl;
    break;
  }
  job.job(); // Ejecutar el trabajo fuera del bloqueo
}
*/

void JobSystem::runTasks() {
  while (true) {
    PrioritizedJob job;
    JobType selectedType;
    bool jobFound = false;

    {
      std::unique_lock<std::mutex> lock(mutex);
      condition.wait(lock, [this]() {
        if (quit) return true;
        for (const auto& [type, queue] : tasks) {
          if (!queue.empty()) return true;
        }
        return false;
        });

      if (quit && tasks.empty()) return; // Salir si `quit` es true y no hay tareas

      // Buscar el trabajo de mayor prioridad entre todas las colas
      job.priority = JobPriority::Low;
      for (auto& [type, queue] : tasks) {
        if (!queue.empty() && queue.top().priority >= job.priority) {
          job.priority = queue.top().priority;
          selectedType = type;
          job = queue.top();  // Asignar el trabajo completo
          jobFound = true;
        }
      }

      // Si encontramos un trabajo, lo removemos de su cola
      if (jobFound) {
        tasks[selectedType].pop();
      }
    }

    // Ejecutar el trabajo fuera del bloqueo
    if (jobFound) {
      job.job(); // Ejecutar el trabajo
    }
  }
}


void JobSystem::addJob(JobType type, JobPriority priority, std::function<void()> job) {
  //CRITICAL SECTION START
  {
    std::lock_guard<std::mutex> lock{ mutex };
    tasks[type].emplace(PrioritizedJob{ priority, std::move(job) });
  }
  condition.notify_one();
  //CRITICAL SECTION END
}