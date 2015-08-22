#include <grid.h>
#include <iostream>

#include <shar.h>

grid::grid() {
    // std::cout << "hello world" << std::endl;
    // _width = 10;
    // std::cout << _width << std::endl;
}

grid::grid(float width, float height, int rect_num) {
    _width = width;
    _height = height;
    _rect_num = rect_num;
    _rect_width = _width/rect_num;
    _rect_height = _height/rect_num;
    
    _points = new vec3[6*_rect_num*_rect_num];

    float x = -_width/2.0;
    float y = _height/2.0;
    int index = 0;
    
    for(int i = 0; i < _rect_num; i++ ) {
        for(int j = 0; j < _rect_num; j++) {

#if 1
            _points[index] = vec3(x, 0, y);

            index++;
            _points[index] = vec3(x+_rect_width, 0, y-_rect_height);

            index++;
            _points[index] = vec3(            x, 0, y-_rect_height);


            index++;
            _points[index] = vec3(            x, 0, y);
            
            index++;
            _points[index] = vec3(x+_rect_width, 0, y);
            
            index++;
            _points[index] = vec3(x+_rect_width, 0, y-_rect_height);

            index++;

#else
            _points[index] = vec3(x, y, 0);

            index++;
            _points[index] = vec3(x+_rect_width, y-_rect_height, 0);

            index++;
            _points[index] = vec3(            x, y-_rect_height, 0);


            index++;
            _points[index] = vec3(            x, y, 0);
            
            index++;
            _points[index] = vec3(x+_rect_width, y, 0);
            
            index++;
            _points[index] = vec3(x+_rect_width, y-_rect_height, 0);

            index++;
#endif
            x += _rect_width;
        }

        y -= _rect_height;
        x = -_width/2.0;
    }


    _drawpoint = 6*_rect_num*_rect_num;

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*6*_rect_num*_rect_num, _points, GL_STATIC_DRAW);

}

int grid::get_point_size() {
    return 6*_rect_num*_rect_num;
}

vec3* grid::get_points() {
    return _points;
}

void grid::render() {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, _drawpoint);
}

void grid::seed_height_map(float* x) {

    for(int i = 0; i < _drawpoint; i++) {
        _points[i].y = sin(i*torad);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*6*_rect_num*_rect_num, _points, GL_STATIC_DRAW);
}

grid::~grid() {
    std::cout << "bye" << std::endl;

    delete [] _points;
}

