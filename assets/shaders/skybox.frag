#version 450 core

in vec3 vert_pos;

uniform samplerCube cubeMap;

out vec4 color;

void main() {
    vec4 cubeMapColor = texture(cubeMap, vert_pos);
    vec4 gradientColor = mix(vec4(0.3f, 0.3f, 0.8f, 1), vec4(0.33f, 0.33f, 0.8f, 1), normalize(vert_pos).y);
    color = cubeMapColor * cubeMapColor.w + gradientColor * (1.0f - cubeMapColor.w);
}