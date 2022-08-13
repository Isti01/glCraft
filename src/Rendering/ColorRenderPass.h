#pragma once

#include "FullscreenQuad.h"
#include "ShaderProgram.h"

class ColorRenderPass {
  Ref<const ShaderProgram> shader;

public:
  explicit ColorRenderPass(const Ref<const ShaderProgram>& shader);

  void setTexture(const std::string& attachmentName, const Ref<Texture>& texture, int32_t slot);
  void render();

  static void renderTextureWithEffect(const Ref<Texture>& texture, const Ref<const ShaderProgram>& effect);
  static void renderTexture(const Ref<Texture>& texture);
};
