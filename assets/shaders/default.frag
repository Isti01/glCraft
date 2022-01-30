#version 450 core

flat in float vert_lighting;
flat in uint animated;
in vec3 vert_pos;
in vec2 vert_uv;
out vec4 color;

uniform vec2 textureAnimation = vec2(0);
uniform sampler2D atlas;

void main() {
    vec2 uv = vert_uv + textureAnimation * animated;
    vec4 texture = texture(atlas, uv / 16);
    if (texture.a < 0.01f) discard;

    color = vec4(texture.xyz * vert_lighting, texture.w);
}