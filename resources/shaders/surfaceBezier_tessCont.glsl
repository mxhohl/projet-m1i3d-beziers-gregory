#version 410

#define MAX_CP 32

layout(vertices=MAX_CP) out;

uniform float uInnerLevel0;
uniform float uInnerLevel1;

uniform float uOuterLevel0;
uniform float uOuterLevel1;
uniform float uOuterLevel2;
uniform float uOuterLevel3;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelInner[0] = uInnerLevel0;
    gl_TessLevelInner[1] = uInnerLevel1;

    gl_TessLevelOuter[0] = uOuterLevel0;
    gl_TessLevelOuter[1] = uOuterLevel1;
    gl_TessLevelOuter[2] = uOuterLevel2;
    gl_TessLevelOuter[3] = uOuterLevel3;
}
