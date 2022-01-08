#version 450 core

in vec3 vert_pos;
in vec2 vert_uv;
out vec4 color;

void main() {
    if (vert_uv.x > .01 && vert_uv.y > .01 && vert_uv.x < .99 && vert_uv.y < .99) discard;

    gl_FragDepth = gl_FragCoord.z - .0001;
    color = vec4(1, 1, 1, 1);
}