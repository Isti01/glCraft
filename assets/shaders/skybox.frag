#version 450 core

in vec3 vert_pos;
out vec4 color;

void main() {
    color = mix(vec4(.3, .3, .8, 1), vec4(.6, .6, .8, 1), normalize(vert_pos).y);
}