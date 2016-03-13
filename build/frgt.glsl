#version 450 core

out vec4 tcolol;
in vec4 color;
in vec4 normalfrag;

layout (location = 2) uniform mat4 proj = mat4(1);
layout (location = 3) uniform mat4 camera = mat4(1);
layout (location = 4) uniform float uns = 0;
in vec4 frag_pos;
in vec4 light_position;
in vec4 cam_pos;
in vec4 vpos_test;

// sh_material lamake = {
//                     vec4(0.1, 0.1, 0.1, 1), //amb
//                     vec4(0.5, 0.5, 0.5, 1), //diffs
//                     vec4(1, 1, 1, 1), //spec
//                     vec4(0.1, 0.1, 1, 1.0), // emissi
//                     100.0
//                     }; //shnini
    
// light firstLight = 
//                 {
//                     vec4(1, 1, 1, 1), //amb color
//                     vec4(1, 1, 1, 1), //diff
//                     vec4(1, 1, 1, 1), //spec
//                     vec4(2, 2, 2, 1) //pos
//                 };

vec4 compnent_wise_product(vec4 a, vec4 b);

void main() {
    //sh_material
    /* if(false) { */
    /*     vec4 mat_amb  =  vec4(0.2, 0, 0, 1)*5; //amb */
    /*     vec4 mat_diff =  vec4(0.5, 0.0, 0.0, 1); //diffs */
    /*     vec4 mat_spec =  vec4(1, 1, 1, 1); //spec */
    /*     vec4 mat_emis =  vec4(0.1, 0.1, 1, 1.0); //emis */
    /*     float shin    =  16; //shin */
    /*  */
    /*     mat4 rotsa = mat4(2*cos(uns*3.14/180.0), 0, -2*sin(uns*3.14/180.0), 0, */
    /*             0, 0, 0, 0, */
    /*             2*sin(uns*3.14/180.0), 0, 2*cos(uns*3.14/180.0), 0, */
    /*             0, 0, 0, 1); */
    /*     //light */
    /*     vec4 light_amb  =  vec4(0.1, 0.1, 0.1, 1)*2; //amb */
    /*     vec4 light_diff =  vec4(0.5, 0.5, 0.5, 1); //diffs */
    /*     vec4 light_spec =  vec4(1, 1, 1, 1); //spec */
    /*     // vec4 light_position =  vec4(0, 0, 10, 1) ; //emis */
    /*  */
    /*  */
    /*  */
    /*     //mat_terms */
    /*     vec4 ambient = compnent_wise_product(mat_amb, light_amb); */
    /*     vec4 diffuse = compnent_wise_product(mat_diff, light_diff); */
    /*     vec4 specular = compnent_wise_product(mat_spec, light_spec); */
    /*  */
    /*     // vec4 normal = normalize(transpose( inverse( camera ) )*normalfrag);  */
    /*     //  */
    /*     vec4 normal = normalize(normalfrag); */
    /*     vec4 light_direction = normalize(light_position-frag_pos); */
    /*     vec4 view_direction = normalize(cam_pos - frag_pos); */
    /*     vec4 half_angle = normalize(view_direction  + light_direction); */
    /*  */
    /*  */
    /*     float spec_angle = max(dot(half_angle, normal), 0.0); */
    /*  */
    /*  */
    /*     vec4 diff_light = diffuse*max(dot(normal, light_direction), 0); */
    /*     vec4 spec = specular*pow(spec_angle, 30); */
    /*  */
    /* //float lalala = ((ambient + diff_light + spec)*color); */
    /* }  */
    // if(vpos_test.z < 0) {
        tcolol = vec4(1);//;(ambient + diff_light + spec)*color;    
    // } else {
        // tcolol = vec4(1, 1, 1, 1);
     // }
}

vec4 compnent_wise_product(vec4 a, vec4 b) {
    return vec4(a.x * b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}
