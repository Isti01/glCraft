#include "Shader.h"

#include "../AssetManager/AssetManager.h"

Shader::Shader(const std::string &name, uint32_t type) {
  id = glCreateShader(type);
  Ref<const std::string> vertexShaderSource = AssetManager::instance().loadText(name);
  const char *vertexShaderSourceStr = vertexShaderSource->c_str();
  glShaderSource(id, 1, &vertexShaderSourceStr, nullptr);
  glCompileShader(id);

  int32_t success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[2048];
    glGetShaderInfoLog(id, sizeof(infoLog) / sizeof(infoLog[0]), nullptr, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

    id = 0;
  }
}

Shader::~Shader() {
  if (isValid()) { glDeleteShader(id); }
}
