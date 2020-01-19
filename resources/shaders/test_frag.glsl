#version 420

out vec4 oFragColor;

uniform vec4 uColor;

void main() {
    oFragColor = vec4(mix(vec3(0.0), uColor.rgb, uColor.a), 1.0);
}
