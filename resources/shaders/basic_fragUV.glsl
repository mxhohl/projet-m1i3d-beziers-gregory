#version 410

in vec2 uv;

out vec4 oFragColor;

uniform vec4 uColor;

void main() {
    // oFragColor = vec4(mix(vec3(0.0), uColor.rgb, uColor.a), 1.0);
    oFragColor = vec4(uv, 0, 1.0);
}
