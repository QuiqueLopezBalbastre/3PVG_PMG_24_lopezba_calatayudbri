#include "ECS/ECSManager.hpp"
#include <iostream>

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

  return newEntity;
}

void ECSManager::destroyEntity(Entity entity)
{

}

bool ECSManager::isEntityAlive(Entity entity) const
{
  if (entity < nextEntity) {
    return true;
  }
  return false;
}

Entity ECSManager::get_nextEntity()
{
  return nextEntity;
}

