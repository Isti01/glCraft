#version 450 core

uniform sampler2D colorTexture;
uniform float power = 2.2;

layout(location = 0) out vec4 color;

void main() {
    vec4 pixel = texelFetch(colorTexture, ivec2(gl_FragCoord.xy), 0);
    color = vec4(pow(pixel.xyz, vec3(1) / power), pixel.w);
}
