#version 450 core

uniform vec4 cam_pos;
uniform bool textured;

in vec4 vert_norm;
in vec4 vert_pos;
in vec4 vert_color;

out vec4 tcolol;

in vec2 uv;

layout(location = 12) uniform sampler2D my_tex;

void main() { 
    vec4 color_point = vec4(0.8, 0.8, 0.8, 1);

    vec4 light_position = cam_pos;
    vec4 direction_to_light = (light_position - vert_pos);
    float legn = length(direction_to_light);
    float diff_con = max(dot((normalize(direction_to_light)),
                              normalize(vert_norm)), 0);

    // tcolol = tcolol*color_point*diff_con;
    /* tcolol = vert_color*color_point*diff_con + vec4(0.1, 0.1, 0.1, 1); */
    if(textured)
        tcolol =  texture(my_tex, vec2(uv.s, uv.t))*color_point*diff_con;
    else
        tcolol =  vert_color*color_point*diff_con + vec4(0.1, 0.1, 0.1, 1);

}

