#version 450 core

uniform sampler2D colorTexture;

layout(location = 0) out vec4 color;

void main() {
    color = texelFetch(colorTexture, ivec2(gl_FragCoord.xy), 0);
}
