#include "ShaderProgram.h"

#include "../AssetManager/AssetManager.h"
#include "Shader.h"

ShaderProgram::ShaderProgram(const std::string &name) {
  Ref<const Shader> vertexShader = AssetManager::instance().loadShader(name + ".vert");
  assert(vertexShader->isValid());

  Ref<const Shader> fragmentShader = AssetManager::instance().loadShader(name + ".frag");
  assert(fragmentShader->isValid());

  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, fragmentShader->getId());
  glAttachShader(shaderProgram, vertexShader->getId());

  glLinkProgram(shaderProgram);
}
int32_t ShaderProgram::getUniformLocation(const std::string &location) const {
  return glGetUniformLocation(shaderProgram, location.c_str());
}

void ShaderProgram::bind() const {
  glUseProgram(shaderProgram);
}

void ShaderProgram::setInt(const std::string &location, int32_t value) const {
  glProgramUniform1i(shaderProgram, getUniformLocation(location), value);
}

void ShaderProgram::setFloat(const std::string &location, float value) const {
  glProgramUniform1f(shaderProgram, getUniformLocation(location), value);
}

void ShaderProgram::setVec3(const std::string &location, const glm::vec3 &value) const {
  glProgramUniform3fv(shaderProgram, getUniformLocation(location), 1, &value.x);
}

void ShaderProgram::setMat4(const std::string &location, const glm::mat4 &value) const {
  glProgramUniformMatrix4fv(shaderProgram, getUniformLocation(location), 1, GL_FALSE, &value[0][0]);
}

ShaderProgram::~ShaderProgram() {
  if (isValid()) { glDeleteProgram(shaderProgram); }
}
void ShaderProgram::setTexture(const std::string &location, const Ref<const Texture> &texture, int32_t slot) const {
  texture->bindToSlot(slot);
  glProgramUniform1i(shaderProgram, getUniformLocation(location), slot);
}
