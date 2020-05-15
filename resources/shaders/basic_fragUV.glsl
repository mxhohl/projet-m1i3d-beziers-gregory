#version 410

in vec2 uv;

out vec4 oFragColor;

void main() {
    oFragColor = vec4(uv, 0, 1.0);
}
