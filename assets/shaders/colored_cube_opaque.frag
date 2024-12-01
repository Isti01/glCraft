#version 450 core

uniform vec4 color = vec4(1, 0, 0, 1);

out vec4 outColor;

void main() {
    outColor = color;
}