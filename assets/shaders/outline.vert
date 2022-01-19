#version 450 core

layout(location = 0) in uint vertexData;

uniform mat4 MVP = mat4(1);

out uint animated;
out vec3 vert_pos;
out vec2 vert_uv;

uint extractByte(uint data, uint offset){
    return (data & (0xffu << offset)) >> offset;
}

uint useValueIfFlag(uint originalValue, uint flagValue, uint flags, uint offset){
    uint flag = (flags >> offset) & 1u;
    return originalValue * (1 - flag) + flagValue * flag;
}

void main() {
    uint xzPos = extractByte(vertexData, 8);
    uint uvCoords = extractByte(vertexData, 16);
    uint flags = extractByte(vertexData, 24);

    animated = useValueIfFlag(0, 1, flags, 0);

    uint yPos = useValueIfFlag(extractByte(vertexData, 0), 256, flags, 1);
    uint xPos = useValueIfFlag(xzPos & 0x0fu, 16, flags, 2);
    uint zPos = useValueIfFlag((xzPos & 0xf0u) >> 4, 16, flags, 3);

    vert_pos = vec3(xPos, yPos, zPos);

    uint xUv = uvCoords & 0x0fu;
    uint yUv = (uvCoords & 0xf0u) >> 4;
    vert_uv = vec2(xUv, yUv);

    gl_Position = MVP * vec4(vert_pos, 1);
}