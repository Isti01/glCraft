#version 450 core

uniform sampler2D colorTexture;

layout(location = 0) out vec4 color;

void main() {
    vec4 pixel = texelFetch(colorTexture, ivec2(gl_FragCoord.xy), 0);
    color = vec4(vec3(1) - pixel.xyz, pixel.w);
}
