#version 450 core

flat in uint textureIndex;

in float vert_lighting;
in vec3 vert_pos;
in vec2 vert_uv;

uniform sampler2DArray atlas;
uniform sampler2D opaqueDepth;
uniform float zFar = 500.0f;
uniform float zNear = 0.1f;

layout(location = 0) out vec4 accumTexture;
layout(location = 1) out vec4 revealageTexture;

float d(float z) {
    return ((zNear * zFar) / z - zFar) / (zNear - zFar);
}

float weight(float z, float a) {
    float b = 1 - d(z);
    return a * max(0.01f, b * b * b * 0.003);
}

void main() {
    if (gl_FragCoord.z > texelFetch(opaqueDepth, ivec2(gl_FragCoord.xy), 0).r + 0.000005){
        discard;
    }
    vec4 texture = texture(atlas, vec3(vert_uv, textureIndex));
    if (texture.a == 1) discard; // opaque objects are already drawn
    accumTexture = vec4(texture.xyz * vert_lighting, texture.w) * weight(gl_FragCoord.z, texture.w);
    revealageTexture = vec4(texture.w);
}