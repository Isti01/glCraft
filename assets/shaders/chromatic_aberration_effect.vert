#version 450 core

layout(location = 0) in vec3 position;

out vec2 vert_pos;
out vec2 vert_uv;

void main() {
    vert_pos = position.xy;
    vert_uv = vert_pos / 2 + 0.5f;
    gl_Position = vec4(position, 1);
}
