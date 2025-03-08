#include "ECS/ECSManager.hpp"
#include "ECS/Component.hpp"
#include <iostream>
#include <algorithm>
Entity ECSManager::createEntity()
{
  Entity newEntity;

  if (!deadEntities_.empty()) {
    newEntity = deadEntities_.back();
    deadEntities_.pop_back();
  }
  else {
    newEntity = nextEntity++;
  }

  for (auto& [key, componentList] : componentListMap_) {
    componentList->grow(nextEntity);
  }

  this->addComponent<NameComponent>(newEntity);
  this->editComponent<NameComponent>(newEntity, [](NameComponent& EntityName) {
      EntityName.name = " ";
      });

  return newEntity;
}

void ECSManager::destroyEntity(Entity entity)
{
  for (auto& [key, componentList] : componentListMap_) {
    componentList->resetComponent(entity);
  }
  deadEntities_.push_back(entity);
  std::reverse(deadEntities_.begin(), deadEntities_.end());
}

bool ECSManager::isEntityAlive(Entity entity) const
{
  // Primero verificamos si la entidad est� dentro del rango v�lido
  if (entity < nextEntity) {
    // Luego verificamos si la entidad est� en la lista de entidades muertas
    auto it = std::find(deadEntities_.begin(), deadEntities_.end(), entity);
    // La entidad est� viva si NO est� en la lista de entidades muertas
    return it == deadEntities_.end();
  }
  return false;
}

Entity ECSManager::get_nextEntity()
{
  return nextEntity;
}

