#ifndef GRID_H
#define GRID_H
#define GLEW_STATIC
// #include <GL\glew.h>
#include <sh_types.h>

class sh_grid {

public:
    sh_grid(float width, float height, int rect_size);
    sh_grid();
    ~sh_grid();

    void render(uint32 vpos, uint32 color,
                uint32 normal, uint32 model_tran,
                uint32 texture, uint32 textured_bool_loc);

    void seed_height_map(float *x);
    void set_position(vec4 pos);
    void compute_normals(bool inverse = false);

    void colors(vec4 *colors, int32 size);

    GLuint _pos_vbo    = 0;
    GLuint _normal_vbo = 0;
    GLuint _color_vbo  = 0;
    uint32 _uv_vbo     = 0;
    uint32 _texture_id = 0;

    float _width;
    float _height;
    float _rect_width;
    float _rect_height;

    int _drawpoint;
    int _rect_num;
    int _num_points;


    vec4 *_points; //Note(sharo): the positions
    vec4 *_data; //Note(sharo): the vertecies. 
    vec4 *_normals;
    vec4 *_colors;

    mat4 _transformation;
    vec4 _position;
};

#endif // GRID_H
