#pragma once

#include "PostProcessEffect.h"

class GammaCorrectionEffect : public PostProcessEffect {
  float power = 0.85;

public:
  GammaCorrectionEffect(bool enabled)
      : PostProcessEffect(AssetManager::instance().loadShaderProgram("assets/shaders/gamma_correction"), enabled){};

  void update() override { shader->setFloat("power", power); }

  void renderGui() override {
    ImGui::Checkbox("Enable gamma correction", &enabled);

    if (enabled) {
      ImGui::SliderFloat("Gamma correction power", &power, 0.5f, 3.0f);
    }
  }
};
