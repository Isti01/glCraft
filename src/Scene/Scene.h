#pragma once
#include <utility>

#include "../Rendering/ShaderProgram.h"
#include "../Rendering/VertexArray.h"
#include "../glCraft.h"
#include "Camera.h"
#include "Entity.h"

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
  void update(float deltaTime);
  void render();
  void renderGui();
};
