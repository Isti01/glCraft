#pragma once

#include "ShaderProgram.h"

class ProceduralShader {
protected:
  [[nodiscard]] virtual std::string emitVertexShaderSource() const = 0;
  [[nodiscard]] virtual std::string emitFragmentShaderSource() const = 0;

public:
  explicit operator Ref<const ShaderProgram>() const { return getShader(); };
  [[nodiscard]] Ref<const ShaderProgram> getShader() const;

  virtual ~ProceduralShader() = default;
};
