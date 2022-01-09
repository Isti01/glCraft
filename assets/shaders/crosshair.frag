#version 450 core

out vec4 color;

void main() {
    gl_FragDepth = -5.0f;
    color = vec4(1, 1, 1, 0.9f);
}