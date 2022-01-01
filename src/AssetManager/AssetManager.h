#pragma once
#include "../Rendering/Shader.h"
#include "../Rendering/ShaderProgram.h"
#include "../glCraft.h"
#include "AssetRegistry.h"
#include "ShaderProgramRegistry.h"
#include "ShaderRegistry.h"
#include "TextRegistry.h"

class AssetManager {
  TextRegistry textRegistry;
  ShaderRegistry shaderRegistry;
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
  Ref<const Shader> loadShader(const std::string &name) { return shaderRegistry.get(name); };
  Ref<const ShaderProgram> loadShaderProgram(const std::string &name) { return shaderProgramRegistry.get(name); };
};
