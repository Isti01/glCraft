#version 450 core

layout(location = 0) in uint vertexData;

uniform mat4 MVP = mat4(1);
uniform vec3 lightDirection = vec3(1, 1, 1);
uniform uint textureAnimation = 0u;

flat out uint textureIndex;

out float vert_lighting;
out vec3 vert_pos;
out vec2 vert_uv;

void main() {
    uint animated = bitfieldExtract(vertexData, 28, 1);

    textureIndex = bitfieldExtract(vertexData, 20, 8) + textureAnimation * animated;

    uint yPos = bitfieldExtract(vertexData,  0, 9);
    uint xPos = bitfieldExtract(vertexData,  9, 5);
    uint zPos = bitfieldExtract(vertexData, 14, 5);
    vert_pos = vec3(xPos, yPos, zPos);

    uint xUv = bitfieldExtract(vertexData, 19, 1);
    uint yUv = bitfieldExtract(vertexData, 20, 1);
    vert_uv = vec2(xUv, yUv);

    uint occlusionLevel = bitfieldExtract(vertexData, 29, 2);
    vert_lighting = 0.75f + 0.08f * occlusionLevel;
    gl_Position = MVP * vec4(vert_pos, 1);
}