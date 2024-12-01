#pragma once

#include "../glCraft.h"
#include "Shader.h"
#include "Texture.h"

class ShaderProgram {
  uint32_t shaderProgram = 0;

  [[nodiscard]] int32_t getUniformLocation(const std::string &location) const;

public:
  ShaderProgram(const Ref<const Shader>& vertexShader, const Ref<const Shader>& fragmentShader);
  explicit ShaderProgram(const std::string &name);
  ~ShaderProgram();

  [[nodiscard]] bool isValid() const { return shaderProgram; };
  void bind() const;

  void setInt(const std::string &location, int32_t value) const;
  void setUInt(const std::string &location, int32_t value) const;
  void setFloat(const std::string &location, float value) const;
  void setVec2(const std::string &location, const glm::vec2 &value) const;
  void setVec3(const std::string &location, const glm::vec3 &value) const;
  void setVec4(const std::string &location, const glm::vec4 &value) const;
  void setMat4(const std::string &location, const glm::mat4 &value) const;
  void setTexture(const std::string &location, const Ref<const Texture> &texture, int32_t slot) const;

  ShaderProgram(const ShaderProgram &) = delete;
  ShaderProgram(ShaderProgram &) = delete;
  ShaderProgram(ShaderProgram &&) noexcept = delete;
  ShaderProgram &operator=(ShaderProgram &) = delete;
  ShaderProgram &operator=(ShaderProgram &&) noexcept = delete;
};
