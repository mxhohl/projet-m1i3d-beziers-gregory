#version 410

layout(vertices=4) out;

uniform float uOuterLevel0;
uniform float uOuterLevel1;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelOuter[0] = uOuterLevel0;
    gl_TessLevelOuter[1] = uOuterLevel1;

}