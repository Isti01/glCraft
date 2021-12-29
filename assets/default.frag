#version 450 core

in vec3 vert_pos;
out vec4 color;

void main() {
    color = vec4(vert_pos + 0.5, 1);
}