#version 410

#define MAX_CP_U 8
#define MAX_CP_V 8
#define MAX_CP 32

layout (quads, equal_spacing, ccw) in;

uniform uint uCPUCount;
uniform uint uCPVCount;

out vec3 normal;
out vec3 position;

vec4 deCasteljau1D(vec4 cp[MAX_CP], uint cp_count,
                   uint offset, uint stride, float t, out vec4 normal_comp);
vec4 deCasteljau2D(uint cp_u_count, uint cp_v_count,
                   float u, float v,
                   out vec3 normal);


void main() {
    if (uCPUCount > 0 && uCPUCount <= MAX_CP_U
        && uCPVCount > 0 && uCPVCount <= MAX_CP_V
        && uCPVCount * uCPUCount <= MAX_CP) {

        vec4 pos = deCasteljau2D(
            uCPUCount, uCPVCount,
            gl_TessCoord.x, gl_TessCoord.y,
            normal
        );

        gl_Position = pos;
        position = pos.xyz / pos.w;

    } else {
        gl_Position = vec4(0., 0., 0., 1.);
    }
}


vec4 linearInterpolation(vec4 a, vec4 b, float t) {
    return (1.0 - t) * a + t * b;
}


vec4 deCasteljau2D(uint cp_u_count, uint cp_v_count,
                   float u, float v,
                   out vec3 normal) {
    vec4 points[MAX_CP];
    vec4 normal_comp[2];

    for (uint i = 0; i < cp_v_count * cp_u_count; ++i) {
        points[i] = gl_in[i].gl_Position;
    }

    for (uint iu = 0; iu < cp_u_count; ++iu) {
        points[iu] = deCasteljau1D(points, cp_v_count,
                                   iu * cp_v_count, 1, v,
                                   normal_comp[0]);
    }

    vec4 point = deCasteljau1D(points, cp_u_count, 0, 1, u, normal_comp[1]);

    normal = normalize(cross(
        normal_comp[0].xyz / normal_comp[0].w,
        normal_comp[1].xyz / normal_comp[1].w
    ));

    return point;

}

vec4 deCasteljau1D(vec4 cp[MAX_CP], uint cp_count,
                   uint offset, uint stride, float t, out vec4 normal_comp) {
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

        if (points_count == 2) {
            normal_comp = points[1] - points[0];
        }

        --points_count;
    }

    return points[0];
}
