#pragma once
#include "../../glCraft.h"
#include "../ShaderProgram.h"
#include "AssetRegistry.h"
#include "TextRegistry.h"

class AssetManager {
  TextRegistry textRegistry;
  AssetManager() = default;

public:
  AssetManager(const AssetManager &) = delete;
  AssetManager(AssetManager &) = delete;
  AssetManager(AssetManager &&) = delete;

  static AssetManager &instance() {
    static AssetManager manager;
    return manager;
  };
  Ref<const std::string> loadText(const std::string &name) { return textRegistry.get(name); };
};
