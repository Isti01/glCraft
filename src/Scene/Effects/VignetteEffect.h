#pragma once

#include "PostProcessEffect.h"

class VignetteEffect : public PostProcessEffect {
  float vignetteIntensity = 2.9;
  float vignetteStart = 2;

public:
  VignetteEffect(bool enabled)
      : PostProcessEffect(AssetManager::instance().loadShaderProgram("assets/shaders/vignette_effect"), enabled) {}

  void update() override {
    shader->setFloat("intensity", vignetteIntensity);
    shader->setFloat("start", vignetteStart);
  }

  void renderGui() override {
    ImGui::Checkbox("Enable vignette effect", &enabled);

    if (enabled) {
      float invertedIntensity = 4 - vignetteIntensity;
      if (ImGui::SliderFloat("Vignette intensity", &invertedIntensity, 1, 3)) {
        vignetteIntensity = 4 - invertedIntensity;
      }

      ImGui::SliderFloat("Vignette start", &vignetteStart, 0, 3);
    }
  }
};
