#version 450 core

in vec3 position;
out vec3 vert_pos;

void main() {
    vert_pos = position;
    gl_Position = vec4(position, 1);
}