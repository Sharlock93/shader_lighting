#version 450

layout(location = 1) in vec4 vpos;

void main() {
    gl_Position = vec4(sin(gl_VertexID), 0, 0, 1);
}
