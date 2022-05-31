#version 450 core

layout(depth_less) out float gl_FragDepth;

out vec4 color;

void main() {
    gl_FragDepth = -gl_FragCoord.z - 5.0f;
    color = vec4(1, 1, 1, 0.9f);
}