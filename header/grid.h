#ifndef GRID_H
#define GRID_H
#define GLEW_STATIC
#include <GL\glew.h>
#include <shar.h>

class grid {

public:
    grid(float width, float height, int rect_size);
    grid();
    ~grid();

    void render();
    vec3* get_points();
    int get_num_points();
    void seed_height_map(float *x);
    void compute_normals();

    GLuint _vbo;
    GLuint _index_vbo;
    GLuint _normal_vbo;
    int _drawpoint;
    float _width;
    float _height;
    int _rect_num;
    int _num_points;

    float _rect_width;
    float _rect_height;

    vec3 *_points;
    vec3 *_normals;

};

#endif // GRID_H
