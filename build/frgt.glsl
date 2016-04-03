#version 450 core

out vec4 tcolol;
in vec4 color;
in vec4 normalfrag;

in vec2 uv;
in vec4 vert_pos;

uniform sampler2D tex_sampler;

vec4 compnent_wise_product(vec4 a, vec4 b) {
    return vec4(a.x * b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}
void main() { 
        // tcolol = texture(tex_sampler, vec2(uv.x, 1-uv.y)) + vec4(0.1, 0.1, 0.1, 1);
    if(vert_pos.y > 15)  {
        tcolol =  vec4(1);
    } else if( vert_pos.y > 10 ) {
        tcolol = vec4(116/255.0, 79/255.0, 4/255.0, 1);
    } else if( vert_pos.y > 5 )  { 
        tcolol = vec4(0, 0.85, 0, 1);
    } else { 
        tcolol = vec4(0, 0.5, 0, 1);
    }

}

