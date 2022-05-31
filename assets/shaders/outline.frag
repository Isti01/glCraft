#version 450 core

layout(depth_less) out float gl_FragDepth;

in vec3 vert_pos;
in vec2 vert_uv;
out vec4 color;

void main() {
    if (vert_uv.x > 0.01f && vert_uv.y > 0.01f && vert_uv.x < 0.99f && vert_uv.y < 0.99f) discard;

    gl_FragDepth = gl_FragCoord.z - 0.00001f;
    color = vec4(1, 1, 1, 1);
}