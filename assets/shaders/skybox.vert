#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 transform = mat4(1);

out vec3 vert_pos;

void main() {
    vec4 pos = transform * vec4(position, 1);
    vert_pos = position;
    gl_Position = pos.xyww;
}