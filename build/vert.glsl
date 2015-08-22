#version 450

layout (location = 0) in vec4 vpos;
layout (location = 2) uniform mat4 proj = mat4(1);
layout (location = 3) uniform mat4 camera = mat4(1);
layout (location = 4) uniform float uns = 50;
layout (location = 5) in vec4 normal;
layout (location = 6) in vec4 vcolor;
layout (location = 7) uniform vec4 cam_poss = vec4(0, 0, 10, 1);
layout (location = 8) uniform vec4 ligh_pos = vec4(-5, 0, 0, 1);
layout (location = 9) uniform mat4 rotsa = mat4(1);

out vec4 color;
out vec4 normalfrag;

out vec4 frag_pos;
out vec4 cam_pos;
out vec4 vpos_test;

vec4 compnent_wise_product(vec4 a, vec4 b) {
    return vec4(a.x * b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}

out vec4 light_position;

void main() {
    
    mat4 trans = mat4(1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);  
  
    color = vcolor;
    normalfrag = camera*vec4(normal.xyz, 0);
    frag_pos = camera*vpos;
    vpos_test = proj*camera*vpos;
    cam_pos = camera*cam_poss;  
    light_position = (camera)*rotsa*ligh_pos;
    gl_Position = proj*camera*vpos;


}
