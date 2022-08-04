#include "ProceduralShader.h"


Ref<const ShaderProgram> ProceduralShader::getShader() const {
  Ref<const Shader> vertexShader = std::make_shared<const Shader>(emitVertexShaderSource(), GL_VERTEX_SHADER);
  Ref<const Shader> fragmentShader = std::make_shared<const Shader>(emitFragmentShaderSource(), GL_FRAGMENT_SHADER);

  return std::make_shared<const ShaderProgram>(vertexShader, fragmentShader);
}
