#version 450 core

uniform sampler2D colorTexture;

layout(location = 0) out vec4 color;


float kernel[] = {
1.0f / sum, 4.0f / sum, 7.0f / sum, 4.0f / sum, 1.0f / sum,
4.0f / sum, 16.0f / sum, 26.0f / sum, 16.0f / sum, 4.0f / sum,
7.0f / sum, 26.0f / sum, 41.0f / sum, 26.0f / sum, 7.0f / sum,
4.0f / sum, 16.0f / sum, 26.0f / sum, 16.0f / sum, 4.0f / sum,
1.0f / sum, 4.0f / sum, 7.0f / sum, 4.0f / sum, 1.0f / sum
};
float sum = 273;
void main() {
    vec3 pixel = vec3(0);
    ivec2 center = ivec2(gl_FragCoord.xy);
    for (int i = -2; i <= 2; ++i) {
        int xKernelIndex = i + 2;
        for (int j = -2; j <= 2; ++j) {
            int yKernelIndex = j + 2;
            ivec2 pixelOffset = center + ivec2(i, j);
            float kernelValue = kernel[xKernelIndex + 5 * yKernelIndex];
            pixel += texelFetch(colorTexture, pixelOffset, 0).rgb * kernelValue;
        }
    }

    vec4 centerPixel = texelFetch(colorTexture, center, 0);
    color = vec4(pixel, centerPixel.a);
}
