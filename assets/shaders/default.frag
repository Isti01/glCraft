#version 450 core

flat in uint animated;
in vec3 vert_pos;
in vec2 vert_uv;
out vec4 color;

uniform vec2 textureAnimation = vec2(0);
uniform sampler2D atlas;

void main() {
    vec2 uv = vert_uv + textureAnimation * animated;
    color = texture(atlas, uv / 16);
    if (color.a < 0.01f) discard;
}