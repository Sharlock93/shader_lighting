#include <grid.h>
#include <iostream>
#include <math.h>
#include <shar.h>
#include <random>

#define CONVERT_TO_INDEX(row, col, weight) ((row)*(weight) + ( col ))
grid::grid() {
    // std::cout << "hello world" << std::endl;
    // _width = 10;
    // std::cout << _width << std::endl;
}

grid::grid(float width, float height, int rect_num) {
    //Todo(sharo): only use 4 points for a quad, not 6
    _width = width;
    _height = height;
    _rect_num = rect_num;
    _rect_width = _width/rect_num;
    _rect_height = _height/rect_num;
    _strid = _rect_num*6;
    
    //Note(sharo): this is stupid, please fix it.
    _points = new vec3[( _rect_num +1)*( _rect_num +1)];

    float x = -_width/2.0;
    float z = -_height/2.0; // Note(sharo): made it in the X, Z plain, so height is Y
    int index = 0;

    for(int i = 0; i <= _rect_num; i++ ) {
        for(int j = 0; j <= _rect_num; j++) {
#if 1
            _points[index++] = vec3(x, 0, z);

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

        z += _rect_height;
        x = -_width/2;
    }


    int *indcies = new int[_rect_num*_rect_num*6];
    int temp_counter = 0;
    int weight = _rect_num + 1;
    for(int i = 0; i < _rect_num; ++i) {
        for(int j = 0; j < _rect_num; ++j) { 
            indcies[temp_counter++] = CONVERT_TO_INDEX(i, j,weight); 
            indcies[temp_counter++] = CONVERT_TO_INDEX(i+1, j+1,weight); 
            indcies[temp_counter++] = CONVERT_TO_INDEX(i+1, j,weight); 
            
            indcies[temp_counter++] = CONVERT_TO_INDEX(i, j,weight); 
            indcies[temp_counter++] = CONVERT_TO_INDEX(i, j+1,weight); 
            indcies[temp_counter++] = CONVERT_TO_INDEX(i+1, j+1,weight); 
        }
    }
 
    _num_points = index;

    _drawpoint = 6*_rect_num*_rect_num;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*index, _points, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_index_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6*_rect_num*_rect_num, indcies, GL_STATIC_DRAW);
    delete [] indcies;
}

int grid::get_point_size() {
    return _num_points;
}

vec3* grid::get_points() {
    return _points;
}

void grid::render() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glDrawArrays(GL_TRIANGLES, 0, _drawpoint);
    glDrawElements(GL_TRIANGLES, _drawpoint, GL_UNSIGNED_INT, NULL);
}

bool is_top_row(int num, int rect_num) {
    if(num < rect_num) 
        return true;
    return false;
}

bool is_bottom_row(int num, int rect_num) {
    if(num >= (rect_num)*(rect_num-1))
        return true;
    return false;
}

bool is_left_col(int num, int rect_num) {
    return !(num%rect_num);
}

bool is_right_col(int num, int rect_num) {
    return !(( num  + 1)%rect_num);
}

bool is_edge(int num, int rect_num) {
    return is_top_row(num, rect_num) || is_bottom_row(num, rect_num) || is_left_col(num, rect_num) || is_right_col(num, rect_num);  
}

bool is_corner(int num, int rect_num) {
    return ( is_top_row(num, rect_num) || is_bottom_row(num, rect_num) ) && ( is_left_col(num, rect_num) || is_right_col(num, rect_num));
}

float get_height(int index, vec3 *points) {
    return points[index*6].y;
}


void raise_rect(int index, vec3 *points, float value) {
    points[index].y = value;
}

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> dist(0.0, 5.0);

void step(int x, int y, int num_rect,int weight, vec3* points) {
    int half = ( num_rect/2 ); 
}


void grid::seed_height_map(float x) {

    raise_rect(0, _points, 10); 

    // step(0,0, _rect_num, _rect_num, _points); 
    
    // std::cout << _rect_num/2 << std::endl;
    // int half_step = _rect_num;
    // for(int i = _rect_num; i > 1; i /= 2) { 
    //     for(int j = 0; j < _rect_num-1; j += i) {
    //         for(int k = 0; k < _rect_num-1; k += i) {
    //             // std::cout << "I, J, K: " << i << " " << j << " " << k << std::endl;
    //             step(j, k, half_step, _rect_num, _points);
    //         }
    //     } 
    //     half_step = ceil(half_step/2.0);
    // }


    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*_num_points, _points, GL_STATIC_DRAW);
}

grid::~grid() {
    std::cout << "bye" << std::endl;

    // delete [] _points;
}

