#version 450 core

layout(location = 0) in uint vertexData;

uniform mat4 MVP = mat4(1);
uniform vec3 lightDirection = vec3(1, 1, 1);

flat out float vert_lighting;
flat out uint animated;

out vec3 vert_pos;
out vec2 vert_uv;

vec3 normals[6] = { vec3(0, 1, 0), vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 0, -1), vec3(0, 0, 1), vec3(0, -1, 0) };

void main() {
    animated = (vertexData >> 27) & 1u;

    uint yPos = vertexData & 0x1ffu;
    uint xPos = (vertexData >> 9) & 0x1fu;
    uint zPos = (vertexData >> 14) & 0x1fu;
    vert_pos = vec3(xPos, yPos, zPos);

    uint xUv = (vertexData >> 19) & 0xfu;
    uint yUv = (vertexData >> 23) & 0xfu;
    vert_uv = vec2(xUv, yUv);

    uint normalIndex = (vertexData >> 29) & 7u;
    vec3 normal = normals[normalIndex];

    vert_lighting = min(max(dot(normalize(lightDirection), normal), 0) + 0.85f, 1);
    gl_Position = MVP * vec4(vert_pos, 1);
}