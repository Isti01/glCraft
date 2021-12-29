#pragma once
#include <utility>

#include "../glCraft.h"
#include "Entity.h"
#include "../Rendering/ShaderProgram.h"
#include "../Rendering/VertexArray.h"

class Scene {
  std::vector<Entity> entities;
  bool initialized = false;

  Ref<VertexArray> vao;
  Ref<const ShaderProgram> defaultShader;

public:
  Scene() = default;
  explicit Scene(std::vector<Entity> entities) : entities(std::move(entities)) {}

  void addEntity(Entity &&entity) { entities.emplace_back(entity); };
  void addEntity(const Entity &entity) { entities.emplace_back(entity); };

  void init();
  void update();
  void render();
  void renderGui();
};
