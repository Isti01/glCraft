#pragma once

#include "../../Application/Window.h"
#include "./PostProcessEffect.h"

class CrosshairEffect : public PostProcessEffect {
  float crosshairSize = 0.015f;
  float crosshairVerticalWidth = 0.2f;
  float crosshairHorizontalWidth = 0.15f;

public:
  CrosshairEffect(bool enabled)
      : PostProcessEffect(AssetManager::instance().loadShaderProgram("assets/shaders/crosshair"), enabled) {}
  void renderGui() override {
    ImGui::Checkbox("Enable crosshair", &enabled);
    if (enabled) {
      ImGui::SliderFloat("Crosshair size", &crosshairSize, 0.01, 1);
      ImGui::SliderFloat("Crosshair vertical width", &crosshairVerticalWidth, 0.01, 1);
      ImGui::SliderFloat("Crosshair horizontal width", &crosshairHorizontalWidth, 0.01, 1);
    }
  }

  void update() override {
    auto& window = Window::instance();
    auto width = window.getWindowWidth();
    auto height = window.getWindowHeight();
    float aspectRatio = width == 0 || height == 0 ? 0 : static_cast<float>(width) / static_cast<float>(height);

    shader->setFloat("size", crosshairSize);
    shader->setFloat("verticalWidth", crosshairVerticalWidth);
    shader->setFloat("horizontalWidth", crosshairHorizontalWidth);
    shader->setFloat("aspectRatio", aspectRatio);
  }
};
