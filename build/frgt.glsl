#version 450 core

out vec4 tcolol;
in vec4 vert_color;
void main() { 
    tcolol = vert_color;//vert_color;
}

