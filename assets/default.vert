#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

uniform mat4 MVP = mat4(1);

out vec3 vert_pos;
out vec2 vert_uv;

void main() {
    vert_uv = uv;
    vert_pos = position;
    gl_Position = MVP * vec4(vert_pos, 1);
}