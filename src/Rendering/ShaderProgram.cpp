#include "ShaderProgram.h"

#include "../AssetManager/AssetManager.h"
#include "../Performance/Trace.h"
#include "Shader.h"

ShaderProgram::ShaderProgram(const Ref<const Shader> &vertexShader, const Ref<const Shader> &fragmentShader) {
  TRACE_FUNCTION();
  assert(vertexShader->isValid());
  assert(fragmentShader->isValid());

  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, fragmentShader->getId());
  glAttachShader(shaderProgram, vertexShader->getId());

  glLinkProgram(shaderProgram);

  int32_t success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[2048];
    glGetProgramInfoLog(shaderProgram, sizeof(infoLog) / sizeof(infoLog[0]), nullptr, infoLog);
    std::cerr << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;

    shaderProgram = 0;
  }
}

ShaderProgram::ShaderProgram(const std::string &name)
    : ShaderProgram(AssetManager::instance().loadShader(name + ".vert"),
                    AssetManager::instance().loadShader(name + ".frag")) {}

ShaderProgram::~ShaderProgram() {
  if (isValid()) {
    glDeleteProgram(shaderProgram);
  }
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

void ShaderProgram::setUInt(const std::string &location, int32_t value) const {
  glProgramUniform1ui(shaderProgram, getUniformLocation(location), value);
}

void ShaderProgram::setFloat(const std::string &location, float value) const {
  glProgramUniform1f(shaderProgram, getUniformLocation(location), value);
}

void ShaderProgram::setVec2(const std::string &location, const glm::vec2 &value) const {
  glProgramUniform2fv(shaderProgram, getUniformLocation(location), 1, &value.x);
}

void ShaderProgram::setVec3(const std::string &location, const glm::vec3 &value) const {
  glProgramUniform3fv(shaderProgram, getUniformLocation(location), 1, &value.x);
}
void ShaderProgram::setVec4(const std::string &location, const glm::vec4 &value) const {
  glProgramUniform4fv(shaderProgram, getUniformLocation(location), 1, &value.x);
}

void ShaderProgram::setMat4(const std::string &location, const glm::mat4 &value) const {
  glProgramUniformMatrix4fv(shaderProgram, getUniformLocation(location), 1, GL_FALSE, &value[0][0]);
}
void ShaderProgram::setTexture(const std::string &location, const Ref<const Texture> &texture, int32_t slot) const {
  texture->bindToSlot(slot);
  glProgramUniform1i(shaderProgram, getUniformLocation(location), slot);
}
