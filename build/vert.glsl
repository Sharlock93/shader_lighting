#version 450

layout (location = 0) in vec4 vpos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec4 normal;

layout (location = 3) uniform mat4 model_tran = mat4(1);
layout (location = 4) uniform mat4 proj = mat4(1);
layout (location = 5) uniform mat4 camera;// = mat4(1);

layout (location = 6) uniform vec4 cam_pos;
layout (location = 7) uniform bool textured = false;
layout (location = 8) in vec2 uv_vert;


out vec4 vert_norm;
out vec4 vert_pos;
out vec4 vert_color;

out vec2 uv;


void main() {
    vert_pos = model_tran*vpos;
    vert_color = color;
    
    uv = uv_vert;

    vec4 nors = normal;
    nors.w = 0;
    mat4 lels = mat4(transpose(inverse(mat3(model_tran))));
    vert_norm = lels*normal;
    gl_Position = proj*camera*model_tran*vpos; 
}
