#pragma once

#include "PostProcessEffect.h"

class InvertEffect : public PostProcessEffect {
public:
  InvertEffect(bool enabled)
      : PostProcessEffect(AssetManager::instance().loadShaderProgram("assets/shaders/invert_effect"), enabled){}

  void update() override {}

  void renderGui() override {
    ImGui::Checkbox("Enable invert effect", &enabled);
  }
};
