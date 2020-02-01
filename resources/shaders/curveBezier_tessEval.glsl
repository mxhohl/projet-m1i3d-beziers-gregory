#version 410

layout (isolines, equal_spacing) in;

#define MAX_CP 8

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
    vec4 points0[MAX_CP];
    vec4 points1[MAX_CP];
    uint points_count[2];
    uint current = 1;

    for (uint i = 0; i < cp_count; ++i) {
        points0[i] = gl_in[i].gl_Position;
    }
    points_count[0] = cp_count;
    points_count[1] = cp_count; /* TODO: pourquoi cp_count et pas cp_count-1 ??????? */

    while (points_count[current] > 1) {
        for (uint i = 0; i < points_count[current]; ++i) {
            if (current == 0) {
                points0[i] = linearInterpolation(points1[i], points1[i + 1], t);
            } else {
                points1[i] = linearInterpolation(points0[i], points0[i + 1], t);
            }
        }

        current = 1 - current;
        points_count[current] = points_count[1 - current] - 1;
    }

    return current == 0 ? points1[0] : points0[0];
}
