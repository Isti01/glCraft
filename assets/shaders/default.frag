#version 450 core

flat in uint textureIndex;

in float vert_lighting;
in vec3 vert_pos;
in vec2 vert_uv;

out vec4 color;

uniform sampler2DArray atlas;

void main() {
    vec4 texture = texture(atlas, vec3(vert_uv, textureIndex));
    if (texture.a < 0.01f) discard;

    color = vec4(texture.xyz * vert_lighting, texture.w);
}