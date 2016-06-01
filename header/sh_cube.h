#ifndef SH_CUBE_H
#define SH_CUBE_H

#include <sh_types.h>
#include <sh_bmp_reader.h>

struct sh_cube {
    sh_cube();
    void render(uint32 vpos, uint32 color,
                uint32 normal, uint32 model_tran,
                uint32 texture, uint32 textured_bool_loc);
    void move_to(vec4 position);
    void move_to(float x, float y, float z);
    void set_size(float scale_x, float scale_y, float scale_z);
    void colors(vec4 color[36]); 
    void compute_normals(bool inverse);
    void compute_normals();
    void set_rotationaxis(vec4 axis);
    void set_rotationpos(vec4 pos);
    void rotate(float angle);
    vec3 get_size();
    void read_texture(const char *filename,
                           uint32 start_x,
                           uint32 start_y,
                           uint32 texture_size);
    void read_texture(sh_bmp_img img);

    uint32 pos_vbo;
    uint32 norm_vbo;
    uint32 color_vbo;
    uint32 uv_vbo;
    uint32 texture_id;
    
    vec4 _position;
    vec3 _scale;

    vec4 _rotaxis;
    vec4 _rotpos;
    
    mat4 _transformation;
    
    vec4 _verts[36];
    vec4 _norms[36];
    vec4 _color[36];
    vec2 _uv[36];
};

#endif
