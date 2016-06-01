#define GLEW_STATIC
#include <sh_cone.h>
#include <math.h>
#include <iostream>

sh_cone::sh_cone(int32 faces, int32 height, int32 radius) {
    _faces = faces;
    float seg_ang = 360.0/(_faces);
    double to_rad = 3.14159265/180.0;
    _number_points = _faces*6;
    _height = height;

    _points = new vec4[_number_points];
    _normals = new vec4[_number_points];
    _colors = new vec4[_number_points];

    vec4 apex(0, height, 0, 1);
    vec4 circle_center(0, 0, 0, 1);
    int32 angle = 0;

    for(int32 i = 0; i < _faces*6; i += 6) {
        _points[i] = apex;
        _points[i + 1] = _points[i + 4] = 
            vec4(radius*cos((angle)*seg_ang*to_rad), 0,
                 radius*sin((angle)*seg_ang*to_rad), 1);

        _points[i + 2] = _points[i + 5] = 
            vec4(radius*cos((angle+1)*seg_ang*to_rad), 0,
                 radius*sin((angle+1)*seg_ang*to_rad), 1);

        _points[i + 3] = circle_center;                        
        angle++;
    }


    _transformation = mat4(1);

    glGenBuffers(1, &pos_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, _number_points*sizeof(vec4),
                 _points, GL_STATIC_DRAW);

    glGenBuffers(1, &norm_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, norm_vbo);
    glBufferData(GL_ARRAY_BUFFER, _number_points*sizeof(vec4),
                 NULL, GL_STATIC_DRAW);

    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, _number_points*sizeof(vec4),
                 NULL, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sh_cone::move_to(vec4 pos) {
    _transformation = shatranslate(pos)*
                      shatranslate(-_position)*
                      _transformation;
    _position = pos;
}

void sh_cone::compute_normals() {
    vec4 apex(0, _height, 0, 1);
    vec4 center(0, 0, 0, 1);
    for(int32 i = 0; i < _number_points; i += 6) {
        vec4 a = _points[i + 1]; 
        vec4 b = _points[i + 2]; 

        vec4 norm_1 = normalize(cross(b - apex, a - apex));
        vec4 norm_2 = normalize(cross(a - center, b - center));
        norm_1.w = 0;
        norm_2.w = 0;
        _normals[i + 0] = norm_1;
        _normals[i + 1] = norm_1;
        _normals[i + 2] = norm_1;

        _normals[i + 3] = norm_2;
        _normals[i + 4] = norm_2;
        _normals[i + 5] = norm_2;
    } 
    //
    glBindBuffer(GL_ARRAY_BUFFER, norm_vbo);
    glBufferData(GL_ARRAY_BUFFER, _number_points*sizeof(vec4),
                _normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sh_cone::colors(vec4 *colors) {
    for(int32 i = 0; i < _number_points; ++i) {
        _colors[i] = colors[i]; 
    }

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, _number_points*sizeof(vec4),
                 _colors, GL_STATIC_DRAW);
}

void sh_cone::render(uint32 vpos, uint32 color,               
                    uint32 normal, uint32 model_tran,
                    uint32 texture, uint32 textured_bool_loc)
{
    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);     
    glEnableVertexAttribArray(vpos);
    glVertexAttribPointer(vpos, 4, GL_FLOAT, GL_FALSE, 0, 0);
    //
    glBindBuffer(GL_ARRAY_BUFFER, norm_vbo);
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glEnableVertexAttribArray(color);
    glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, 0); 

    glUniformMatrix4fv(model_tran, 1, GL_TRUE, &_transformation[0][0]);
    //
    glDrawArrays(GL_TRIANGLES, 0, _number_points);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}
