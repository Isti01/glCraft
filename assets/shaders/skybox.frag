#version 450 core

in vec3 vert_pos;
out vec4 color;

void main() {
    color = mix(vec4(0.3f, 0.3f, 0.8f, 1), vec4(0.33f, 0.33f, 0.8f, 1), normalize(vert_pos).y);
}