#version 450 core

flat in uint textureIndex;

in float vert_lighting;
in vec3 vert_pos;
in vec2 vert_uv;

uniform sampler2DArray atlas;

out vec4 color;

void main() {
    vec4 texture = texture(atlas, vec3(vert_uv, textureIndex));
    if (texture.a < 1) discard; // only keep opaque objects
    color = vec4(texture.xyz * vert_lighting, texture.w);
}