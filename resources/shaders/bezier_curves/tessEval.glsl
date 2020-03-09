#version 410

layout (isolines, equal_spacing) in;

#define MAX_CP 32

uniform uint uCPCount;


vec4 deCasteljau(uint cp_count, float t);

void main() {
    if (uCPCount > 0 && uCPCount <= MAX_CP) {
        gl_Position = deCasteljau(uCPCount, gl_TessCoord.x);
    } else {
        gl_Position = vec4(0., 0., 0., 1.);
    }
}

vec4 linearInterpolation(vec4 a, vec4 b, float t) {
    return (1.0 - t) * a + t * b;
}

vec4 deCasteljau(uint cp_count, float t) {
    vec4 points[MAX_CP];
    uint points_count;

    for (uint i = 0; i < cp_count; ++i) {
        points[i] = gl_in[i].gl_Position;
    }
    points_count = cp_count;

    while (points_count > 1) {
        for (uint i = 0; i < points_count; ++i) {
            points[i] = linearInterpolation(points[i], points[i + 1], t);
        }

        --points_count;
    }

    return points[0];
}
