#version 450 core

out vec4 tcolol;
in vec4 color;
in vec4 normalfrag;

layout (location = 8) uniform vec4 ligh_pos;
in vec4 frag_pos;

void main() {
    //light
    vec4 light_color = vec4(1, 1, 1, 1)*5;
    vec4 amb = vec4(0.1, 0.1, 0.1, 1);
    vec4 diff2 =  frag_pos - ligh_pos;
    float attn = 1.0/(pow(length(diff2), 2));

    vec4 light_dir = normalize(diff2);
    vec4 diff_light = light_color*max(dot(normalfrag, light_dir), 0)*attn;


    vec4 diff = vec4(1, 1, 1, 1);

    vec4 spec = vec4(0, 0, 0, 1);

    tcolol = (diff_light + amb + spec)*diff;
    // tcolol = (normalfrag + 1.0)/2.0;
}



