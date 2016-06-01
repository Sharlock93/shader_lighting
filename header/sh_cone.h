#ifndef SH_CONE_H
#define SH_CONE_H

#include <GL\glew.h>
#include <sh_types.h>
#include <sh_bmp_reader.h>
#include <Shar\headers\sharfun.h>

struct sh_cone {
    sh_cone(int32 faces = 8, int32 height = 1, int32 radius = 1);

    void render(uint32 vpos, uint32 color,
                uint32 normal, uint32 model_tran,
                uint32 texture, uint32 textured_bool_loc);
    void move_to(vec4 position);
    void move_to(float x, float y, float z);
    void set_size(float scale_x, float scale_y, float scale_z);
    void colors(vec4 color[36]); 
    void compute_normals();
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

    int32 _number_points;
    int32 _faces;
    int32 _height;
    
    mat4 _transformation;  
    vec4 *_points;
    vec4 *_normals;
    vec4 *_colors;
};

#endif /* ifndef SH_CONE_H */

