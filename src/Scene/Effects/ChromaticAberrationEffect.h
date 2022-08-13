#pragma once

#include "PostProcessEffect.h"

class ChromaticAberrationEffect : public PostProcessEffect {
  float aberrationStart = 1.5f;
  float aberrationROffset = 0.005;
  float aberrationGOffset = 0.01;
  float aberrationBOffset = -0.005;

public:
  ChromaticAberrationEffect(bool enabled)
      : PostProcessEffect(AssetManager::instance().loadShaderProgram("assets/shaders/chromatic_aberration_effect"),
                          enabled) {}

  void renderGui() override {
    ImGui::Checkbox("Enable chromatic aberration effect", &enabled);

    if (enabled) {
      ImGui::SliderFloat("Aberration start", &aberrationStart, 0.5, 3);
      ImGui::SliderFloat("Aberration R Offset", &aberrationROffset, -0.01, 0.01);
      ImGui::SliderFloat("Aberration G Offset", &aberrationGOffset, -0.01, 0.01);
      ImGui::SliderFloat("Aberration B Offset", &aberrationBOffset, -0.01, 0.01);
    }
  }

  void update() override {
    shader->setFloat("start", aberrationStart);
    shader->setFloat("rOffset", aberrationROffset);
    shader->setFloat("gOffset", aberrationGOffset);
    shader->setFloat("bOffset", aberrationBOffset);
  }
};
