#version 450 core

uniform sampler2D colorTexture;
uniform float intensity = 2;
uniform float start = 2.0f;

in vec3 vert_pos;

layout(location = 0) out vec4 color;

void main() {
    vec4 pixel = texelFetch(colorTexture, ivec2(gl_FragCoord.xy), 0);
    float effect = pow(distance(vert_pos / start, vec3(0)), intensity);
    color = vec4(mix(pixel.xyz, vec3(0), effect), pixel.w);
}
