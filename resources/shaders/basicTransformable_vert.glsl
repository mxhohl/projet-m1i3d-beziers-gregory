#version 410

layout(location = 0) in vec3 iPosition;

uniform mat4 projMatrix;
uniform mat4 mvMatrix;

void main() {
    gl_Position = projMatrix * mvMatrix * vec4(iPosition, 1.0);
}
