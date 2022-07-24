#include "ColorRenderPass.h"

ColorRenderPass::ColorRenderPass(const Ref<const ShaderProgram>& shader) : shader(shader) {
  assert(shader != nullptr);
};

void ColorRenderPass::setTexture(const std::string& attachmentName, const Ref<Texture>& texture, int32_t slot) {
  shader->setTexture(attachmentName, texture, slot);
};

void ColorRenderPass::render() {
  shader->bind();
  glDisable(GL_DEPTH_TEST);
  FullscreenQuad::getVertexArray()->renderIndexed();
  glEnable(GL_DEPTH_TEST);
}
