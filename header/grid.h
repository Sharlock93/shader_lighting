#ifndef GRID_H
#define GRID_H
#define GLEW_STATIC
#include <GL\glew.h>
#include <shar.h>
#include <sh_types.h>

class grid {

public:
    grid(float width, float height, int rect_size);
    grid();
    ~grid();

    void render(uint32 vpos, uint32 color,
                  uint32 normal, uint32 model_tran,
                  uint32 texture, uint32 textured_bool_loc);
    vec4* get_points();
    int get_num_points();
    void seed_height_map(float *x);
    void set_position(vec4 pos);
    void compute_normals(bool inverse = false);
    void test_compute_color();
    void colors(vec4 *colors, int32 size);

    GLuint _pos_vbo = 0;
    GLuint _normal_vbo = 0;
    GLuint _color_vbo = 0;
    uint32 _uv_vbo = 0;
    uint32 _texture_id = 0;

    int _drawpoint;
    float _width;
    float _height;
    int _rect_num;
    int _num_points;

    float _rect_width;
    float _rect_height;

    vec4 *_points; //Note(sharo): the positions
    vec4 *_data; //Note(sharo): the vertecies. 
    vec4 *_normals;
    vec4 *_colors;

    mat4 _transformation;
    vec4 _position;

};

#endif // GRID_H
