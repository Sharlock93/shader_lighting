#include <grid.h>
#include <iostream>
#include <math.h>
#include <Shar\headers\shar.h>
#include <random>


#define INDEX(r, c, w) ((r)*(w) + c)

grid::grid() {

}

grid::grid(float width, float height, int rect_num) {
    //Todo(sharo): only use 4 points for a quad, not 6
    _width = width;
    _height = height;
    _rect_num = rect_num;
    _rect_width = _width/rect_num;
    _rect_height = _height/rect_num;
    
    //Note(sharo): this is stupid, please fix it.
    _points = new vec3[( _rect_num +1)*( _rect_num +1)];

    float x = -_width/2.0;
    float z = -_height/2.0; 
    int index = 0;

    for(int i = 0; i <= _rect_num; i++ ) {
        for(int j = 0; j <= _rect_num; j++) {
            _points[index++] = vec3(x, 0, z);
            x += _rect_width;
        }
        z += _rect_height;
        x = -_width/2;
    }


    int *indcies = new int[_rect_num*_rect_num*6];
    int temp_counter = 0;
    int weight = _rect_num + 1;

    //Note(sharo): two triangles per quad, two normals
    //Todo(sharo): normals can't be shared?
    _normals = new vec3[_rect_num*_rect_num*6]; 

    for(int i = 0; i < _rect_num; ++i) {
        for(int j = 0; j < _rect_num; ++j) { 
            indcies[temp_counter++] = INDEX(i, j,weight); 
            indcies[temp_counter++] = INDEX(i+1, j+1,weight); 
            indcies[temp_counter++] = INDEX(i+1, j,weight);
            indcies[temp_counter++] = INDEX(i, j,weight); 
            indcies[temp_counter++] = INDEX(i, j+1,weight); 
            indcies[temp_counter++] = INDEX(i+1, j+1,weight);  
        }
    }
 
    _num_points = index;

    _drawpoint = 6*_rect_num*_rect_num;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*index, _points, GL_DYNAMIC_DRAW);

    //
    // glGenBuffers(1, &_index_vbo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_vbo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6*_rect_num*_rect_num, indcies, GL_STATIC_DRAW);
    delete [] indcies;

    glGenBuffers(1, &_normal_vbo);
}

int grid::get_num_points() {
    return _num_points;
}

vec3* grid::get_points() {
    return _points;
}

void grid::compute_normals() {
    int normal_index = 0;
    for(int i = 0; i < _rect_num; ++i) {
        for(int j = 0; j < _rect_num; ++j) { 
            
            vec3 a = _points[INDEX(i,j,weight)];
            vec3 b = _points[INDEX(i+1,j+1,weight)];
            vec3 c = _points[INDEX(i+1,j,weight)];
            vec3 d = _points[INDEX(i,j+1,weight)];

            vec3 norm_1 = normalize(-cross(b-a, c-a)); 
            vec3 norm_2 =  normalize(cross(b-a, d-a));

            _normals[normal_index++] = norm_1;
            _normals[normal_index++] = norm_1; 
            _normals[normal_index++] = norm_1; 

            _normals[normal_index++] = norm_2;
            _normals[normal_index++] = norm_2;
            _normals[normal_index++] = norm_2;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, _normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*_rect_num*_rect_num*6, _normals, GL_DYNAMIC_DRAW);
}

void grid::render() {
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glDrawElements(GL_TRIANGLES, _drawpoint, GL_UNSIGNED_INT, NULL);
}

void grid::seed_height_map(float *x) {
    //Note(sharo): fill only the number of points in the 
    //grid
    for(int i = 0; i < _num_points; ++i) {
        _points[i].y = x[i];
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*_num_points, _points, GL_STATIC_DRAW);
}

grid::~grid() {
    std::cout << "bye" << std::endl;

    // delete [] _points;
}

