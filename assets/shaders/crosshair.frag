#version 450 core

in vec2 vert_pos;

uniform sampler2D colorTexture;
uniform float size;
uniform float horizontalWidth;
uniform float verticalWidth;
uniform float aspectRatio;

layout(location = 0) out vec4 color;


void main() {
    vec4 pixel = texelFetch(colorTexture, ivec2(gl_FragCoord.xy), 0);

    float scaledY = vert_pos.y;
    float scaledX = vert_pos.x * aspectRatio;

    bool isInBox = scaledY >= -size && scaledY <= size && scaledX >= -size && scaledX <= size;

    float crosshairHorizontalWidth = size * horizontalWidth;
    float crosshairVerticalWidth = size * verticalWidth;

    bool isInHorizontalCross = scaledY >= -crosshairHorizontalWidth && scaledY <= crosshairHorizontalWidth;
    bool isinVerticalCross = scaledX >= -crosshairVerticalWidth && scaledX <= crosshairVerticalWidth;
    bool shouldInvert = isInBox && (isInHorizontalCross || isinVerticalCross);

    int t = int(shouldInvert);
    vec3 invertedColor = t * (vec3(1) - pixel.xyz * 0.5f);
    vec3 normalCoplor = (1 - t) * pixel.xyz;
    color =  vec4(invertedColor + normalCoplor, pixel.w);
}
