#include "ColorRenderPass.h"

#include "../AssetManager/AssetManager.h"

ColorRenderPass::ColorRenderPass(const Ref<const ShaderProgram>& shader) : shader(shader) {
  assert(shader != nullptr);
}

void ColorRenderPass::setTexture(const std::string& attachmentName, const Ref<Texture>& texture, int32_t slot) {
  shader->setTexture(attachmentName, texture, slot);
}

void ColorRenderPass::render() {
  shader->bind();
  glDisable(GL_DEPTH_TEST);
  FullscreenQuad::getVertexArray()->renderIndexed();
  glEnable(GL_DEPTH_TEST);
}

void ColorRenderPass::renderTextureWithEffect(const Ref<Texture>& texture, const Ref<const ShaderProgram>& effect) {
  ColorRenderPass renderPass(effect);
  renderPass.setTexture("colorTexture", texture, 0);
  renderPass.render();
}

void ColorRenderPass::renderTexture(const Ref<Texture>& texture) {
  static Ref<const ShaderProgram> colorIdentity = AssetManager::instance().loadShaderProgram("assets/shaders/identity");
  renderTextureWithEffect(texture, colorIdentity);
}
