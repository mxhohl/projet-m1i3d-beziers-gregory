#version 420

layout(vertices=3) out;

uniform float uInnerLevel;
uniform float uOuterLevel0;
uniform float uOuterLevel1;
uniform float uOuterLevel2;

void main() {
    gl_TessLevelInner[0] = uInnerLevel;

    gl_TessLevelOuter[0] = uOuterLevel0;
    gl_TessLevelOuter[1] = uOuterLevel1;
    gl_TessLevelOuter[2] = uOuterLevel2;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
