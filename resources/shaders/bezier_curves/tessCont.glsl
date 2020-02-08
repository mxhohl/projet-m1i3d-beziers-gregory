#version 410

#define MAX_CP 8

layout(vertices=MAX_CP) out;

uniform float uOuterLevel0;
uniform float uOuterLevel1;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelOuter[0] = 1;
    gl_TessLevelOuter[1] = uOuterLevel1;
}
