#version 450 core

in vec3 vert_pos;
in vec2 vert_uv;
out vec4 color;

uniform sampler2D atlas;

void main() {
    color = texture(atlas, vert_uv / 16);
}