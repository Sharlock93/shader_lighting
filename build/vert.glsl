#version 450

layout (location = 0) in vec4 vpos;
layout (location = 2) uniform mat4 proj = mat4(1);
layout (location = 3) uniform mat4 camera;// = mat4(1);
layout (location = 5) in vec4 normal;
layout (location = 6) in vec4 vcolor;
layout (location = 7) in vec2 vert_uv;
out vec2 uv;
out vec4 vert_pos;
vec4 compnent_wise_product(vec4 a, vec4 b) {
    return vec4(a.x * b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}

void main() {
    uv = vert_uv;
    vert_pos = vpos;
    gl_Position = proj*camera*vpos;
    
}
