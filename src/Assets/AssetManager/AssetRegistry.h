#pragma once
#include "../../glCraft.h"
#include "../ShaderProgram.h"

template<typename T>
class AssetRegistry {
  std::unordered_map<std::string, WeakRef<const T>> registry;
  virtual Ref<const T> loadAsset(const std::string &name) = 0;

public:
  [[nodiscard]] bool hasAsset(const std::string &name) const;
  Ref<const T> get(const std::string &name);
};

template<typename T>
bool AssetRegistry<T>::hasAsset(const std::string &name) const {
  if (!registry.contains(name)) return false;
  return !registry.at(name).expired();
}

template<typename T>
Ref<const T> AssetRegistry<T>::get(const std::string &name) {
  if (hasAsset(name)) {
    WeakRef<const T> ref = registry.at(name);
    return ref.lock();
  }

  Ref<const T> asset = loadAsset(name);
  if (asset != nullptr) { registry[name] = asset; }

  return asset;
}
