#version 410

layout(vertices=20) out;

uniform float uLevel;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelInner[0]
        = gl_TessLevelInner[1]
        = gl_TessLevelOuter[0]
        = gl_TessLevelOuter[1]
        = gl_TessLevelOuter[2]
        = gl_TessLevelOuter[3]
        = uLevel;
}
