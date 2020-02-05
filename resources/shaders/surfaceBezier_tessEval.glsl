#version 410

#define MAX_CP_U 8
#define MAX_CP_V 8
#define MAX_CP 32

layout (quads, equal_spacing, ccw) in;

uniform uint uCPUCount;
uniform uint uCPVCount;


vec4 deCasteljau1D(vec4 cp[MAX_CP], uint cp_count, uint offset, float t);
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


vec4 bilinearInterpolation(vec4 p00, vec4 p01, vec4 p10, vec4 p11,
                           float u, float v) {
    vec4 p1 = (1.0 - u) * p00 + u * p01;
    vec4 p2 = (1.0 - u) * p10 + u * p11;
    return (1.0 - v) * p1 + v * p2;
}

vec4 linearInterpolation(vec4 a, vec4 b, float t) {
    return (1.0 - t) * a + t * b;
}


vec4 deCasteljau2D(uint cp_u_count, uint cp_v_count, float u, float v) {
    vec4 points0[MAX_CP];
    vec4 points1[MAX_CP];
    uint points_count[4];
    uint current = 1;

    for (uint i = 0; i < cp_u_count * cp_v_count; ++i) {
        points0[i] = gl_in[i].gl_Position;
    }
    points_count[0] = cp_u_count; // u for 0
    points_count[1] = cp_v_count; // v for 0
    points_count[2] = cp_u_count; // u for 1
    points_count[3] = cp_v_count; // v for 1
    
    while (points_count[current * 2] > 1 && points_count[current * 2 + 1] > 1) {
        for (int i = 0; i < points_count[current * 2]; ++i) {
            for (int j = 0; j < points_count[current * 2 + 1]; ++j) {
                if (current == 0) {
                    points0[i * cp_v_count + j] = bilinearInterpolation(
                        points1[i * cp_v_count + j],
                        points1[i * cp_v_count + j + 1],
                        points1[(i + 1) * cp_v_count + j],
                        points1[(i + 1) * cp_v_count + j + 1],
                        u, v
                    );
                } else {
                    points1[i * cp_v_count + j] = bilinearInterpolation(
                        points0[i * cp_v_count + j],
                        points0[i * cp_v_count + j + 1],
                        points0[(i + 1) * cp_v_count + j],
                        points0[(i + 1) * cp_v_count + j + 1],
                        u, v
                    );
                }
            }
        }

        current = 1 - current;
        points_count[current * 2] = points_count[(1 - current) * 2] - 1;
        points_count[current * 2 + 1] = points_count[(1 - current) * 2 + 1] - 1;
    }

    if (current == 0) {
        if (points_count[2] > 1) {
            return deCasteljau1D(points1, points_count[2], cp_v_count, u);
        } else if (points_count[3] > 1) {
            return deCasteljau1D(points1, points_count[3], 0, v);
        } else {
            return points1[0];
        }
    } else {
        if (points_count[0] > 1) {
            return deCasteljau1D(points0, points_count[0], cp_v_count, u);
        } else if (points_count[1] > 1) {
            return deCasteljau1D(points0, points_count[1], 0, v);
        } else {
            return points0[0];
        }
    }
}

vec4 deCasteljau1D(vec4 cp[MAX_CP], uint cp_count, uint offset, float t) {
    vec4 points0[MAX_CP];
    vec4 points1[MAX_CP];
    uint points_count[2];
    uint current = 1;

    for (uint i = 0; i < cp_count; ++i) {
        points0[i] = cp[i * (offset + 1)];
    }
    points_count[0] = cp_count;
    points_count[1] = cp_count;

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
