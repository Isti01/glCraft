#pragma once
#include "../Rendering/ShaderProgram.h"
#include "../glCraft.h"
#include "AssetRegistry.h"
#include "ShaderProgramRegistry.h"
#include "TextRegistry.h"

class AssetManager {
  TextRegistry textRegistry;
  ShaderProgramRegistry shaderProgramRegistry;

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
  Ref<const ShaderProgram> loadShader(const std::string &name) { return shaderProgramRegistry.get(name); };
};
