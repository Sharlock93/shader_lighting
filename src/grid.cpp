#include <grid.h>
#include <iostream>
#include <math.h>
#include <Shar\headers\shar.h>
#include <Shar\headers\sharfun.h>
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
    
    _position = vec4(0, 0, 0, 1);
    
    //Note(sharo): this is stupid, please fix it.
    _points = new vec4[( _rect_num +1)*( _rect_num +1)];

    float x = -_width/2.0;
    float z = -_height/2.0; 
    int index = 0;

    for(int i = 0; i <= _rect_num; i++ ) {
        for(int j = 0; j <= _rect_num; j++) {
            _points[index++] = vec4(x, 0, z, 1);
            x += _rect_width;
        }
        z += _rect_height;
        x = -_width/2;
    }


    _data = new vec4[_rect_num*_rect_num*6];
    _colors = new vec4[_rect_num*_rect_num*6];

    int temp_counter = 0;
    int weight = _rect_num + 1;

    //Note(sharo): two triangles per quad, two normals
    //Todo(sharo): normals can't be shared? i.e: unique for each
    //vertex
    _normals = new vec4[_rect_num*_rect_num*6]; 

    for(int i = 0; i < _rect_num; ++i) {
        for(int j = 0; j < _rect_num; ++j) { 
            _data[temp_counter++] = _points[INDEX(i, j,weight)];
            _data[temp_counter++] = _points[INDEX(i+1, j+1,weight)]; 
            _data[temp_counter++] = _points[INDEX(i+1, j,weight)];
            _data[temp_counter++] = _points[INDEX(i, j,weight)]; 
            _data[temp_counter++] = _points[INDEX(i, j+1,weight)]; 
            _data[temp_counter++] = _points[INDEX(i+1, j+1,weight)];  
        }
    }
 
    _num_points = index;
    _drawpoint = 6*_rect_num*_rect_num;

    glGenBuffers(1, &_pos_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*_drawpoint, _data, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_normal_vbo);
    glGenBuffers(1, &_color_vbo);
}

int grid::get_num_points() {
    return _num_points;
}

void grid::colors(vec4 *colors, int32 size) {
    for(int i = 0; i < size; ++i) {
        _colors[i] = colors[i];
    } 
    glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*_drawpoint,
                   _colors, GL_STATIC_DRAW);
}

vec4* grid::get_points() {
    return _points;
}

void grid::test_compute_color() {
    for(int i = 0; i < _drawpoint; ++i ) {
        if(_data[i].y > 15)  {
            _colors[i] =  vec4(1, 1, 1, 1);
        } else if( _data[i].y > 10 ) {
            _colors[i] = vec4(116/255.0, 79/255.0, 4/255.0, 1);
        } else if( _data[i].y > 5 )  { 
            _colors[i] = vec4(0, 0.85, 0, 1);
        } else { 
            _colors[i] = vec4(0, 0.6117, 0, 1);
        }
    } 

    glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*_drawpoint, _colors, GL_STATIC_DRAW);
}

void grid::compute_normals(bool inverse) {
    int normal_index = 0;
    int weight = _rect_num + 1;
    for(int i = 0; i < _rect_num; ++i) {
        for(int j = 0; j < _rect_num; ++j) { 
            
            vec4 a = _points[INDEX(i,j, weight)];
            vec4 b = _points[INDEX(i+1,j+1, weight)];
            vec4 c = _points[INDEX(i+1,j, weight)];
            vec4 d = _points[INDEX(i,j+1, weight)];

            vec4 norm_1 = normalize(cross(c-a, b-a)); 
            vec4 norm_2 =  normalize(cross(b-a, d-a));
            if(inverse){
                norm_1 = norm_1*-1;
                norm_2 = norm_2*-1;
            }
            norm_1.w = norm_2.w = 0;

            _normals[normal_index++] = norm_1;
            _normals[normal_index++] = norm_1; 
            _normals[normal_index++] = norm_1; 

            _normals[normal_index++] = norm_2;
            _normals[normal_index++] = norm_2;
            _normals[normal_index++] = norm_2;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, _normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*_rect_num*_rect_num*6, _normals, GL_DYNAMIC_DRAW);
}

void grid::set_position(vec4 pos) {
    _transformation = shatranslate(pos)*
                      shatranslate(-_position)*
                      _transformation; 
    _position = pos; 
}

void grid::render(uint32 vpos, uint32 color,
                  uint32 normal, uint32 model_tran,
                  uint32 texture, uint32 textured_bool_loc) {
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _pos_vbo);

    glEnableVertexAttribArray(vpos);
    glVertexAttribPointer(vpos, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _normal_vbo);
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
    glEnableVertexAttribArray(color);
    glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, 0);

    if((_texture_id == 0) || (_uv_vbo == 0)) {
        glUniform1i(textured_bool_loc, 0);
    }

    
    //Todo(sharo): maybe make _transformation useful
    glUniformMatrix4fv(model_tran,
                      1, GL_TRUE,
                      &_transformation[0][0]);

    // glDrawElements(GL_TRIANGLES, _drawpoint, GL_UNSIGNED_INT, NULL);
    glDrawArrays(GL_TRIANGLES, 0, _drawpoint);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void grid::seed_height_map(float *x) {
    //Note(sharo): fill only the number of points in the 
    //grid
    int weight = _rect_num+1;
    for(int i = 0; i < _num_points; ++i) {
        _points[i].y = x[i];
    }

    int temp_counter = 0;
    for(int i = 0; i < _rect_num; ++i) {
        for(int j = 0; j < _rect_num; ++j) { 
            _data[temp_counter++] = _points[INDEX(i, j,weight)];
            _data[temp_counter++] = _points[INDEX(i+1, j+1,weight)]; 
            _data[temp_counter++] = _points[INDEX(i+1, j,weight)];
            _data[temp_counter++] = _points[INDEX(i, j,weight)]; 
            _data[temp_counter++] = _points[INDEX(i, j+1,weight)]; 
            _data[temp_counter++] = _points[INDEX(i+1, j+1,weight)];  
        }
    }


    glBindBuffer(GL_ARRAY_BUFFER, _pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*_drawpoint, _data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

grid::~grid() {
    std::cout << "bye" << std::endl;

    // delete [] _points;
}

