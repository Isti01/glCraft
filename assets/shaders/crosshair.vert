#version 450 core

layout(location = 0) in vec3 position;

out vec2 vert_pos;

void main() {
    vert_pos = position.xy;
    gl_Position = vec4(position, 1);
}
