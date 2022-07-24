#ifndef VIGNETTEEFFECT_H
#define VIGNETTEEFFECT_H

#include "../Rendering/FrameBuffer.h"
#include "../Rendering/ShaderProgram.h"

class PostProcessEffect {
  Ref<const ShaderProgram> shader;
  Ref<Framebuffer> framebuffer;

public:
  explicit PostProcessEffect(const Ref<const ShaderProgram>& shader);

  Ref<const ShaderProgram> getShader() { return shader; };

  void render();
};

#endif
