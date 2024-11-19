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

      if (quit && tasks.empty()) return; // Salir si no quedan trabajos y se ha solicitado terminar

      // Buscar el trabajo de mayor prioridad en todas las listas
      for (auto& [type, list] : tasks) {
        if (!list.empty()) {
          if (!jobFound || list.front().priority > job.priority) {
            job = list.front();
            selectedType = type;
            jobFound = true;
          }
        }
      }

      // Si encontramos un trabajo, lo eliminamos de su lista
      if (jobFound) {
        tasks[selectedType].pop_front();
      }
    }

    // Ejecutar el trabajo fuera del bloqueo
    if (jobFound) {
      job.job(); // Ejecutar el trabajo
    }
  }
}


void JobSystem::addJob(JobType type, JobPriority priority, std::function<void()> job) {
  {
    std::lock_guard<std::mutex> lock(mutex);

    // Añadir el nuevo trabajo a la lista correspondiente
    tasks[type].emplace_back(PrioritizedJob{ priority, std::move(job) });

    // Ordenar la lista de trabajos de mayor a menor prioridad
    tasks[type].sort([](const PrioritizedJob& a, const PrioritizedJob& b) {
      return a.priority > b.priority;  // Prioridad alta primero
      });
  }
  condition.notify_one(); // Notificar a un hilo de que hay un nuevo trabajo
}