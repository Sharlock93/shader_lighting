#include <grid.h>
#include <iostream>
#include <math.h>
#include <Shar\headers\shar.h>
#include <random>

// #define DEBUG


#include "fractmod.c"

int corner = 5;
float H = 1;
float rough = 0;
grid::grid() {
    // std::cout << "hello world" << std::endl;
    // _width = 10;
    // std::cout << _width << std::endl;
}

inline int convert_to_index(int row, int col, int weight) {
    if(row > weight) row++; 
    if(row <= -1) row--; 

    if(col <= -1) col--; 
    if(col > weight) col++;

    row = row%weight;
    col = col%weight;


    if(row < 0) {
        while(row < 0) row += weight;
    }

    if(col < 0) { 
        while(col < 0)
            col += weight;
    }

    return (row)*(weight) + ( col );
}

inline int convert_to_index(vec2 i, int weight) {
    return convert_to_index(i.x, i.y, weight); 
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
    float z = -_height/2.0; // Note(sharo): made it in the X, Z plain, so height is Y
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
    for(int i = 0; i < _rect_num; ++i) {
        for(int j = 0; j < _rect_num; ++j) { 
            indcies[temp_counter++] = convert_to_index(i, j,weight); 
            indcies[temp_counter++] = convert_to_index(i+1, j+1,weight); 
            indcies[temp_counter++] = convert_to_index(i+1, j,weight); 
            
            indcies[temp_counter++] = convert_to_index(i, j,weight); 
            indcies[temp_counter++] = convert_to_index(i, j+1,weight); 
            indcies[temp_counter++] = convert_to_index(i+1, j+1,weight); 
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
    glDrawElements(GL_TRIANGLES, _drawpoint, GL_UNSIGNED_INT, NULL);
}


float get_height(int index, vec3 *points) {
    return points[index].y;
}


void raise_rect(int index, vec3 *points, float value) {
    points[index].y = value;
}

float get_neigh_average(int row, int col,int num_rect, int weight, vec3 *points) {
    int half =  (num_rect/2.0 );
    //Note(sharo): assumed row, col are the middle point
    
    vec2 top = vec2(row-half, col); 
    vec2 bot = vec2(row+half, col); 
    vec2 lef = vec2(row, col-half); 
    vec2 rih = vec2(row, col+half); 
    int t = convert_to_index(top, weight);
    int l = convert_to_index(lef, weight);
    int r = convert_to_index(rih, weight);
    int b = convert_to_index(bot, weight);


#if 0 
    std::cout << "r,c: " << row << " " << col;
    std::cout << " t,b,l,r: " << t << "|" << top << " " << b << "|" << bot << " " << l << "|" << lef << " " << r << "|" << rih  << std::endl;
#endif

    float ave = (get_height(t, points) + get_height(b, points) + get_height(l, points) + get_height(r, points))/4.0;
    return ave;
}

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> dist;

void square_step(int x, int y, int num_rect, int weight, vec3 *points) { 
    
    int half = num_rect/2;
    vec2 left_index  = vec2(x, y - half);
    vec2 top_index   = vec2(x-half, y);
    vec2 right_index = vec2(x, y+half);
    vec2 bottm_index = vec2(x+half, y);
    
#if DEBUG
    std::cout << "LTRB: " << left_index << " " << top_index << " ";
    std::cout << right_index << " " << bottm_index << " x,y,rn: ";
    std::cout << x << " " << y << " " << num_rect << " " << half << std::endl;
#endif
     
    points[convert_to_index(top_index, weight)].y   = get_neigh_average(top_index.x, top_index.y, num_rect, weight, points)    +dist(mt);
    points[convert_to_index(left_index, weight)].y  = get_neigh_average(left_index.x, left_index.y, num_rect, weight, points)  +dist(mt);
    points[convert_to_index(right_index, weight)].y = get_neigh_average(right_index.x, right_index.y, num_rect, weight, points)+dist(mt);    
    points[convert_to_index(bottm_index, weight)].y = get_neigh_average(bottm_index.x, bottm_index.y, num_rect, weight, points)+dist(mt); 
}

void step(int x, int y, int num_rect,int weight, vec3* points) {
    int half = num_rect/2;
    int middle_index = convert_to_index(half+x, half+y, weight);

    // std::cout << "m: " << middle_index  << std::endl; 
    float c1 = get_height(convert_to_index(x, y, weight), points);
    float c2 = get_height(convert_to_index(x, y+num_rect, weight), points);
    float c3 = get_height(convert_to_index(x+num_rect, y, weight), points);
    float c4 = get_height(convert_to_index(x+num_rect, y+num_rect, weight), points);

#if DEBUG
    std::cout << "index, (c1, c2, c3, c4): " << middle_index << " " << c1;
    std::cout << " " << c2 << " " << c3 << " " << c4 << std::endl;
#endif

    points[middle_index].y = (c1 + c2 + c3 + c4)/4.0 + dist(mt); 
    
}




void grid::seed_height_map(float x) {
    // std::cout << "hello" << std::endl;
    // Note(sharo): other implementation
    // float *arr = alloc2DFractArray (_rect_num);
    // fill2DFractArray (arr, _rect_num, 0, 50,0.3);
    // for(int i = 0; i < _num_points; ++i) {
    //     _points[i].y = arr[i];
    // }
 
    rough = x;
    int corner = 2;
    dist = decltype(dist)(-rough, rough);
    
    raise_rect(0, _points, corner); 
    raise_rect(convert_to_index(0, _rect_num, _rect_num+1), _points, corner); 
    raise_rect(convert_to_index(_rect_num, 0, _rect_num+1), _points, corner); 
    raise_rect(convert_to_index(_rect_num, _rect_num, _rect_num+1), _points, corner); 
 

    int half_step = _rect_num;
    //Note(sharo): repeat until the length is bigger than 1
    for(int i = _rect_num; i > 1; i /= 2) { 
        for(int j = 0; j < _rect_num; j += i) {  
            for(int k = 0; k < _rect_num; k += i) {
                // cout << "doingcords: " << j << "|" << k << endl;
                step(j, k, half_step, _rect_num+1, _points);
            }  
        }
        
        for(int j = 0; j < _rect_num; j += i) {
            for(int k = 0; k < _rect_num; k += i) {            
                square_step(j+half_step/2, k+half_step/2, half_step, _rect_num+1, _points);
            }
        } 

        rough *= pow(2, -H);
        dist = decltype(dist)(-rough, rough);
        half_step = ceil(half_step/2.0);
    }


    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*_num_points, _points, GL_STATIC_DRAW);
}

grid::~grid() {
    std::cout << "bye" << std::endl;

    // delete [] _points;
}

