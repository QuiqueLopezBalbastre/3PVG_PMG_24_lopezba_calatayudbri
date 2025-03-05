#ifndef __ECS_MANAGER__
#define __ECS_MANAGER__ 1

#include <vector>
#include <optional>
#include <functional>
#include <unordered_map>
#include <memory>
#include <iostream>

struct ComponentListBase {
  virtual ~ComponentListBase() {};
  virtual void grow(unsigned int current_entities) = 0;
};

template <typename T>
struct ComponentList : ComponentListBase {
  std::vector<std::optional<T>> componentlist;
  virtual void grow(unsigned int current_entities) override;
};

using Entity = unsigned int;

class ECSManager {
public:
  const Entity INVALID_Entity = 0;

  Entity createEntity();
  void destroyEntity(Entity entity);
  bool isEntityAlive(Entity entity) const;
  Entity get_nextEntity();
  template<typename T>
  void addComponentType();
  template<typename T>
  std::optional<T*> getComponent(size_t entity);
  template<typename T>
  int addComponent(size_t entity);
  template<typename T>
  bool editComponent(size_t entity, const std::function<void(T&)>& editor);

private:
  typedef std::unordered_map<size_t, std::unique_ptr<ComponentListBase>> map_type;
  map_type componentListMap_;

  Entity nextEntity = 1;
  std::vector<Entity> deadEntities_;
};

template<typename T>
void ECSManager::addComponentType() {
  size_t key = typeid(T).hash_code();
  std::unique_ptr<ComponentList<T>> value = std::make_unique<ComponentList<T>>();
  componentListMap_.emplace(key, std::move(value));
}

template<typename T>
std::optional<T*> ECSManager::getComponent(size_t entity) {
  size_t hash = typeid(T).hash_code();
  auto it = componentListMap_.find(hash);

  if (it == componentListMap_.end()) {
    return std::nullopt; // Tipo de componente no encontrado
  }

  ComponentList<T>* componentList = static_cast<ComponentList<T>*>(it->second.get());

  if (entity >= componentList->componentlist.size() || !componentList->componentlist[entity].has_value()) {
    return std::nullopt; // La entidad no tiene el componente o está fuera de rango
  }

  return &componentList->componentlist[entity].value(); // Devuelve un puntero al componente
}

template<typename T>
int ECSManager::addComponent(size_t entity)
{
  size_t hash = typeid(T).hash_code();
  map_type::iterator it = componentListMap_.find(hash);
  if (it == componentListMap_.end()) {
    std::cerr << "ERROR: Component list not found";
    return -1;
  }
  ComponentList<T>* cl = static_cast<ComponentList<T>*>(it->second.get());
  if (entity >= cl->componentlist.size()) {
    cl->grow(nextEntity);
  }

  cl->componentlist[entity] = T{};
  return 0;
}

template <typename T>
bool ECSManager::editComponent(size_t entity, const std::function<void(T&)>& editor) {
  size_t hash = typeid(T).hash_code();
  auto it = componentListMap_.find(hash);
  if (it == componentListMap_.end()) {
    std::cerr << "ERROR: Component type not found\n";
    return false;
  }

  ComponentList<T>* componentList = static_cast<ComponentList<T>*>(it->second.get());

  // Expandir el vector si la entidad excede el tamaño actual
  if (entity >= componentList->componentlist.size()) {
    componentList->grow((unsigned int)entity + 1); // Crece para incluir la entidad
  }

  // Inicializar el componente si es nulo
  if (!componentList->componentlist[entity].has_value()) {
    componentList->componentlist[entity] = T{}; // Crear un nuevo componente por defecto
  }

  // Editar el componente
  editor(componentList->componentlist[entity].value());
  return true;
}

template<typename T>
void ComponentList<T>::grow(unsigned int current_entities) {
  size_t current_size = componentlist.size();
  size_t component_size = sizeof(T);
  size_t new_size = 0;

  if (componentlist.empty())
  {
    new_size = 1;
  }
  else if (current_size == (component_size * current_entities)) {
    return;
  }
  else {
    new_size = component_size * current_entities;
  }
  componentlist.resize(new_size, std::nullopt);
}

#endif
