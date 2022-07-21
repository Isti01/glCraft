#version 450 core

layout(location = 0) in uint vertexData;

uniform mat4 MVP = mat4(1);

out vec3 vert_pos;
out vec2 vert_uv;

void main() {
    uint yPos = bitfieldExtract(vertexData,  0, 9);
    uint xPos = bitfieldExtract(vertexData,  9, 5);
    uint zPos = bitfieldExtract(vertexData, 14, 5);
    vert_pos = vec3(xPos, yPos, zPos);

    uint xUv = bitfieldExtract(vertexData, 19, 1);
    uint yUv = bitfieldExtract(vertexData, 20, 1);
    vert_uv = vec2(xUv, yUv);

    gl_Position = MVP * vec4(vert_pos, 1);
}