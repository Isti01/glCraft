#include "PostProcessEffect.h"

#include "../Application/Window.h"
#include "../Rendering/ColorRenderPass.h"

PostProcessEffect::PostProcessEffect(const Ref<const ShaderProgram>& shader) : shader(shader) {}

void PostProcessEffect::render() {
  Window& window = Window::instance();
  int32_t width = window.getWindowWidth();
  int32_t height = window.getWindowHeight();
  if (framebuffer == nullptr || framebuffer->getWidth() != width || framebuffer->getHeight() != height) {
    framebuffer = std::make_shared<Framebuffer>(width, height, false, 1);
  }

  Ref<FramebufferStack> framebufferStack = window.getFramebufferStack();
  Ref<Framebuffer> colorSource = framebufferStack->peek();
  framebufferStack->push(framebuffer, 1);
  ColorRenderPass::renderTextureWithEffect(colorSource->getColorAttachment(0), shader);

  Ref<Framebuffer> resultFbo = framebufferStack->pop();
  ColorRenderPass::renderTexture(resultFbo->getColorAttachment(0));
}
