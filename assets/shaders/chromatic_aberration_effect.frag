#version 450 core

uniform sampler2D colorTexture;

uniform float start = 1;
uniform float rOffset = 0.005;
uniform float gOffset = 0.075;
uniform float bOffset = -0.005;

in vec2 vert_pos;
in vec2 vert_uv;

layout(location = 0) out vec4 color;

void main() {
    float effect = dot(vec2(vert_pos), vec2(vert_pos)) / start;
    color = vec4(
    texture(colorTexture, vert_uv + rOffset * effect).r,
    texture(colorTexture, vert_uv + gOffset * effect).g,
    texture(colorTexture, vert_uv + bOffset * effect).b,
    1
    );
}
