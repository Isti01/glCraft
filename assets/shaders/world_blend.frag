#version 450 core

uniform sampler2D accumTexture;
uniform sampler2D revealageTexture;

out vec4 color;

void main() {
    vec4 accum = texelFetch(accumTexture, ivec2(gl_FragCoord.xy), 0);
    float r = texelFetch(revealageTexture, ivec2(gl_FragCoord.xy), 0).r;
    color = vec4(accum.rgb / clamp(accum.a, 0.0001f, 50000.0f), r);
}
