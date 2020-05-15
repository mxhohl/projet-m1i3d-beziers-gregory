#version 410

#define CP_COUNT 16

uniform mat4 projMatrix;

layout (quads, equal_spacing, ccw) in;

out vec2 uv;

vec4 deCasteljau1D(vec4 cp[CP_COUNT], uint cp_count,
                   uint offset, uint stride, float t);
vec4 deCasteljau2D(vec4 cp[CP_COUNT],
                   uint cp_u_count, uint cp_v_count,
                   float u, float v);

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    uv = vec2(u, v);

    vec4 cp[CP_COUNT];
    for (int i = 0; i < CP_COUNT; ++i) {
        if (i != 5 && i != 6 && i != 9 && i != 10) {
            cp[i] = gl_in[i].gl_Position;
        }
    }

    cp[5] = (u * gl_in[5].gl_Position + v * gl_in[16].gl_Position)
          / (u + v);
    cp[6] = (u * gl_in[6].gl_Position + (1.0 - v) * gl_in[17].gl_Position)
          / (u + (1.0 - v));
    cp[9] = ((1.0 - u) * gl_in[9].gl_Position + v * gl_in[18].gl_Position)
          / ((1.0 - u) + v);
    cp[10] = ((1.0 - u) * gl_in[10].gl_Position + (1.0 - v) * gl_in[19].gl_Position)
           / (2.0 - u - v);

    gl_Position = projMatrix * deCasteljau2D(cp, 4, 4, u, v);
}

vec4 linearInterpolation(vec4 a, vec4 b, float t) {
    return (1.0 - t) * a + t * b;
}


vec4 deCasteljau2D(vec4 cp[CP_COUNT],
                   uint cp_u_count, uint cp_v_count,
                   float u, float v) {
    vec4 points[CP_COUNT];

    for (uint i = 0; i < cp_v_count * cp_u_count; ++i) {
        points[i] = cp[i];
    }

    for (uint iu = 0; iu < cp_u_count; ++iu) {
        points[iu] = deCasteljau1D(points, cp_v_count, iu * cp_v_count, 1, v);
    }

    return deCasteljau1D(points, cp_u_count, 0, 1, u);

}

vec4 deCasteljau1D(vec4 cp[CP_COUNT], uint cp_count,
                   uint offset, uint stride, float t) {
    vec4 points[CP_COUNT];
    uint points_count;

    for (uint i = 0; i < cp_count; ++i) {
        points[i] = cp[offset + i * stride];
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

