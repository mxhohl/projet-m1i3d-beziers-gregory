#version 410

#define MAX_CP_U 8
#define MAX_CP_V 8
#define MAX_CP 32

layout (quads, equal_spacing, ccw) in;

uniform uint uCPUCount;
uniform uint uCPVCount;


vec4 deCasteljau1D(vec4 cp[MAX_CP], uint cp_count,
                   uint offset, uint stride, float t);
vec4 deCasteljau2D(uint cp_u_count, uint cp_v_count, float u, float v);


void main() {
    if (uCPUCount > 0 && uCPUCount <= MAX_CP_U
        && uCPVCount > 0 && uCPVCount <= MAX_CP_V
        && uCPVCount * uCPUCount <= MAX_CP) {
        gl_Position = deCasteljau2D(
            uCPUCount, uCPVCount,
            gl_TessCoord.x, gl_TessCoord.y
        );
    } else {
        gl_Position = vec4(0., 0., 0., 1.);
    }
}


vec4 linearInterpolation(vec4 a, vec4 b, float t) {
    return (1.0 - t) * a + t * b;
}


vec4 deCasteljau2D(uint cp_u_count, uint cp_v_count, float u, float v) {
    vec4 points[MAX_CP];

    for (uint i = 0; i < cp_v_count * cp_u_count; ++i) {
        points[i] = gl_in[i].gl_Position;
    }

    for (uint iu = 0; iu < cp_u_count; ++iu) {
        points[iu] = deCasteljau1D(points, cp_v_count, iu * cp_v_count, 1, v);
    }

    return deCasteljau1D(points, cp_u_count, 0, 1, u);

}

vec4 deCasteljau1D(vec4 cp[MAX_CP], uint cp_count,
                   uint offset, uint stride, float t) {
    vec4 points[MAX_CP];
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
