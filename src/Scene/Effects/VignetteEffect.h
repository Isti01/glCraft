#ifndef VIGNETTEEFFECT_H
#define VIGNETTEEFFECT_H

#include "PostProcessEffect.h"

class VignetteEffect : public PostProcessEffect {
  float vignetteIntensity = 2.9;
  float vignetteStart = 1.5f;

public:
  VignetteEffect(bool enabled)
      : PostProcessEffect(AssetManager::instance().loadShaderProgram("assets/shaders/vignette_effect"), enabled) {}

  void updateUniforms() override {
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


#endif
