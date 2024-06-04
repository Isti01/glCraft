#version 450 core

uniform sampler2D accumTexture;
uniform sampler2D revealageTexture;
uniform sampler2D opaqueTexture;

out vec4 color;

void main() {
    vec4 accum = texelFetch(accumTexture, ivec2(gl_FragCoord.xy), 0);
    float alpha = 1 - texelFetch(revealageTexture, ivec2(gl_FragCoord.xy), 0).r;
    vec3 transparentLayer = accum.rgb / clamp(accum.a, 1e-4, 5e4);
    vec4 opaqueLayer = texelFetch(opaqueTexture, ivec2(gl_FragCoord.xy), 0);
    vec3 composited = transparentLayer.rgb * alpha + opaqueLayer.rgb * (opaqueLayer.a - alpha);
    color = vec4(composited, max(alpha, opaqueLayer.a));
}
