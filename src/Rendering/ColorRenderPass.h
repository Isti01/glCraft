#ifndef COLORRENDERPASS_H
#define COLORRENDERPASS_H

#include "FullscreenQuad.h"
#include "ShaderProgram.h"

class ColorRenderPass {
  Ref<const ShaderProgram> shader;

public:
  ColorRenderPass(const Ref<const ShaderProgram>& shader);

  void setTexture(const std::string& attachmentName, const Ref<Texture>& texture, int32_t slot);
  void render();
};

#endif
