#ifndef INVERTEFFECT_H
#define INVERTEFFECT_H

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


#endif
