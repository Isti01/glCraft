#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 MVP = mat4(1);

out vec3 vert_pos;

void main() {
    vert_pos = position;
    gl_Position = MVP * vec4(vert_pos, 1);
}