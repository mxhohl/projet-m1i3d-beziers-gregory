#version 410

uniform vec3 uLightColor;
uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;

in vec3 normal;
in vec3 position;

out vec4 oFragColor;

vec4 phong(vec3 normal);

void main() {
    oFragColor = phong(normal);
}

vec4 phong(vec3 normal) {
    vec3 lightDir = normalize(vec3(0.0) - position);

    vec3 ia = uLightColor * uKa;
    vec3 id = uLightColor * uKd * vec3(max(0.0, dot(normal, lightDir)));

    // return vec4(ia + id, 1.0);
    return vec4(normal, 0.0);
}
