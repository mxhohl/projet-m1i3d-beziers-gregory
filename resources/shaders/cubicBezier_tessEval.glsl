#version 410

layout (isolines, equal_spacing) in;

void main() {
    /* For Quads */
    /* vec4 p1 = mix(gl_in[1].gl_Position, gl_in[0].gl_Position, gl_TessCoord.x);
    vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
    gl_Position = mix(p1, p2, gl_TessCoord.y); */

    /* For Triangles */
    /* gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position
                 + gl_TessCoord.y * gl_in[1].gl_Position
                 + gl_TessCoord.z * gl_in[2].gl_Position); */

    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;

    float u = gl_TessCoord.x;
    // the basis functions:
    float b0 = (1. - u) * (1. - u) * (1. - u);
    float b1 = 3. * u * (1. - u) * (1. - u);
    float b2 = 3. * u * u * (1. - u);
    float b3 = u * u * u;

    gl_Position = b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
}
