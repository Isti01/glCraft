#version 450 core

layout(location = 0) in uint vertexData;

uniform mat4 MVP = mat4(1);

out vec3 vert_pos;
out vec2 vert_uv;

void main() {
    uint yPos = vertexData & 0x1ffu;
    uint xPos = (vertexData >> 9) & 0x1fu;
    uint zPos = (vertexData >> 14) & 0x1fu;
    vert_pos = vec3(xPos, yPos, zPos);

    uint xUv = (vertexData >> 19) & 0x0fu;
    uint yUv = (vertexData >> 23) & 0x0fu;
    vert_uv = vec2(xUv, yUv);

    gl_Position = MVP * vec4(vert_pos, 1);
}