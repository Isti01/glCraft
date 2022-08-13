#pragma once

#include "../../Rendering/ProceduralShader.h"
#include "PostProcessEffect.h"

class GaussianBlurEffect : public PostProcessEffect {
private:
  int32_t stDev = 2;
  std::map<int32_t, Ref<const ShaderProgram>> shaders;

  Ref<const ShaderProgram> getBlurShader(int32_t blurStDev) {
    if (!shaders.contains(blurStDev)) {
      shaders[blurStDev] = static_cast<Ref<const ShaderProgram>>(GaussianBlurShader(blurStDev));
    }

    return shaders.at(blurStDev);
  }

public:
  GaussianBlurEffect(bool enabled) : PostProcessEffect(nullptr, enabled){};

  void update() override { shader = getBlurShader(stDev); };
  void renderGui() override {
    ImGui::Checkbox("Enable gaussian blur effect", &enabled);

    if (enabled) {
      ImGui::SliderInt("Gaussian Blur StDev: ", &stDev, 0, 5);
    }
  }

private:
  class GaussianBlurShader : public ProceduralShader {
    int32_t stDev;

  protected:
    [[nodiscard]] std::string emitVertexShaderSource() const override {
      return "#version 450 core\n"
             "layout(location = 0) in vec3 position;\n"
             "void main() {\n"
             "    gl_Position = vec4(position, 1);\n"
             "}";
    }
    [[nodiscard]] std::string emitFragmentShaderSource() const override {
      static std::vector<std::vector<uint32_t>> binomials = {{1},
                                                             {1, 2, 1},
                                                             {1, 4, 6, 4, 1},
                                                             {1, 6, 15, 20, 15, 6, 1},
                                                             {1, 8, 28, 56, 70, 56, 28, 8, 1},
                                                             {1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1}};
      static std::vector<float> binomialSums = {1, 16, 256, 4096, 65536, 1048576};
      std::stringstream ss;
      ss << "#version 450 core\n"
            "uniform sampler2D colorTexture;\n"
            "layout(location = 0) out vec4 color;\n"
            "float kernel[] = {\n";
      uint32_t kernelSize = stDev * 2 + 1;

      for (uint32_t i = 0; i < kernelSize; i++) {
        ss << "    ";
        for (uint32_t j = 0; j < kernelSize; j++) {
          if (i || j) {
            ss << ", ";
          }
          ss << (static_cast<float>(binomials[stDev][i] * binomials[stDev][j]) / binomialSums[stDev]);
        }
        ss << '\n';
      }
      ss << "};\n"
            "void main() {\n"
            "    vec3 pixel = vec3(0);\n"
            "    ivec2 center = ivec2(gl_FragCoord.xy);\n"
         << "    for (int i = -" << stDev << "; i <= " << stDev << "; ++i) {\n"
         << "        int xKernelIndex = i + " << stDev << ";\n"
         << "        for (int j = -" << stDev << "; j <= " << stDev << "; ++j) {\n"
         << "            int yKernelIndex = j + " << stDev << ";\n"
         << "            ivec2 pixelOffset = center + ivec2(i, j);\n"
         << "            float kernelValue = kernel[xKernelIndex +" << kernelSize << " * yKernelIndex];\n"
         << "            pixel += texelFetch(colorTexture, pixelOffset, 0).rgb * kernelValue;\n"
            "        }\n"
            "    }\n"
            "    vec4 centerPixel = texelFetch(colorTexture, center, 0);\n"
            "    color = vec4(pixel, centerPixel.a);\n"
            "}";

      return ss.str();
    }

  public:
    GaussianBlurShader(int32_t stDev) : stDev(stDev) { assert(stDev >= 0 && stDev <= 5); };
  };
};
