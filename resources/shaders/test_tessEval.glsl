#version 420

layout (triangles, equal_spacing) in;

void main() {
    /* For Quads */
    /* vec4 p1 = mix(gl_in[1].gl_Position, gl_in[0].gl_Position, gl_TessCoord.x);
    vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
    gl_Position = mix(p1, p2, gl_TessCoord.y); */

    /* For Triangles */
    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position
                 + gl_TessCoord.y * gl_in[1].gl_Position
                 + gl_TessCoord.z * gl_in[2].gl_Position);
}
