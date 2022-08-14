#include "Shader.h"

#include "../AssetManager/AssetManager.h"
#include "../Performance/Trace.h"

Shader::Shader(const std::string &source, uint32_t type) {
  TRACE_FUNCTION();
  assert(type != 0 && "Couldn't identify the shader type");

  id = glCreateShader(type);
  const char *vertexShaderSourceStr = source.c_str();
  glShaderSource(id, 1, &vertexShaderSourceStr, nullptr);
  glCompileShader(id);

  int32_t success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[2048];
    glGetShaderInfoLog(id, sizeof(infoLog) / sizeof(infoLog[0]), nullptr, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;

    id = 0;
  }
}

Shader::Shader(const std::string &name)
    : Shader(*AssetManager::instance().loadText(name),
             name.ends_with(".vert") ? GL_VERTEX_SHADER : (name.ends_with(".frag") ? GL_FRAGMENT_SHADER : 0)) {}

Shader::~Shader() {
  if (isValid()) {
    glDeleteShader(id);
  }
}
