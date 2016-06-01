#define GLEW_STATIC
#include <sh_cube.h>
#include <GL\glew.h>
#include <iostream>
#include <sh_bmp_reader.h>

#include <Shar\headers\sharfun.h>

sh_cube::sh_cube() {
    //Note(sharo): for some weird reason, I can't assign the list to _verts directly
    //its not allowed pre C++11, don't want to use C++11 very much.
    vec4 temp[] =
    { 
        //front 1
        vec4(-1, 1, 1, 1), vec4( 1,-1, 1, 1), vec4(-1,-1, 1, 1),
        vec4(-1, 1, 1, 1), vec4( 1, 1, 1, 1), vec4( 1,-1, 1, 1),

        //right
        vec4( 1, 1, 1, 1), vec4( 1,-1,-1, 1), vec4( 1,-1, 1, 1),
        vec4( 1, 1, 1, 1), vec4( 1, 1,-1, 1), vec4( 1,-1,-1, 1), 
        
        //back
        vec4( 1, 1,-1, 1), vec4(-1,-1,-1, 1), vec4( 1,-1,-1, 1), 
        vec4( 1, 1,-1, 1), vec4(-1, 1,-1, 1), vec4(-1,-1,-1, 1), 
 
        //left
        vec4(-1, 1,-1, 1), vec4(-1,-1, 1, 1), vec4(-1,-1,-1, 1), 
        vec4(-1, 1,-1, 1), vec4(-1, 1, 1, 1), vec4(-1,-1, 1, 1),
 
        //up
        vec4(-1, 1,-1, 1), vec4( 1, 1, 1, 1), vec4(-1, 1, 1, 1), 
        vec4(-1, 1,-1, 1), vec4( 1, 1,-1, 1), vec4( 1, 1, 1, 1),
 
        //down
        vec4(-1,-1, 1, 1), vec4( 1,-1,-1, 1), vec4(-1,-1,-1, 1),
        vec4(-1,-1, 1, 1), vec4( 1,-1, 1, 1), vec4( 1,-1,-1, 1),
    }; 

    for(int i = 0; i < 36; ++i ) {
        _verts[i] = temp[i];   
    }
    
    _position = vec4(0, 0, 0, 1);
    _scale = vec3(1, 1, 1);
    
    pos_vbo = 0;
    norm_vbo = 0;
    color_vbo = 0;
    uv_vbo = 0;
    texture_id = 0;

    glGenBuffers(1, &pos_vbo);
    glGenBuffers(1, &norm_vbo);
    glGenBuffers(1, &color_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_verts),
                 &_verts[0][0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, norm_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_norms), NULL, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_color), NULL, GL_STATIC_DRAW);

}

void sh_cube::set_size(float x, float y, float z) {
    vec3 new_size(x, y, z);
    new_size = new_size/_scale;
    _scale = vec3(x, y, z);

    _transformation = shatranslate(_position)
                      *shascale(new_size.x, new_size.y, new_size.z);
                      *shatranslate(-_position)
                      *_transformation;

}

void sh_cube::colors(vec4 colors[36]) {
    for(int i = 0; i < 36; ++i) {
        _color[i] = colors[i];
    }

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_color),
                 &_color[0][0], GL_STATIC_DRAW);
}
//Todo(sharo): maybe any sized quad instead of a rect 
void sh_cube::read_texture(const char *filename,
                           uint32 start_x,
                           uint32 start_y,
                           uint32 texture_size) {

    sh_bmp_img img = sh_read_bmp(filename);

    uint32 row_u = 0;
    for(int i = 0; i < 6; ++i) {
        vec2 u_c(( i%4)*texture_size + start_x,
                  row_u*texture_size + start_y);

        _uv[i*6+0] = u_c;  
        _uv[i*6+1] = vec2(u_c.x+texture_size, u_c.y+texture_size);  
        _uv[i*6+2] = vec2(u_c.x, u_c.y+texture_size);  

        _uv[i*6+3] = u_c;  
        _uv[i*6+4] = vec2(u_c.x+texture_size, u_c.y);  
        _uv[i*6+5] = vec2(u_c.x+texture_size, u_c.y+texture_size); 

        if(( (i+1)%4 ) == 0) row_u += 1;
    }
 

    for(int i = 0; i < 36; ++i) {
        _uv[i] = _uv[i]/img.width;
        _uv[i].y *= -1;
    }
    
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height,
                  0, GL_BGRA, GL_UNSIGNED_BYTE, img.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenBuffers(1, &uv_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_uv), _uv, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void sh_cube::compute_normals(bool inverse) {
    for(int i = 0; i < 6; ++i) {
        vec4 a = _verts[i*6 + 0]; 
        vec4 c = _verts[i*6 + 1]; 
        vec4 d = _verts[i*6 + 2]; 

        vec4 norm_face = normalize(cross(d-a, c-a));
        
        if(inverse) {
            norm_face = -1*norm_face;
        }
        norm_face.w = 0;

        _norms[i*6 + 0] = norm_face;
        _norms[i*6 + 1] = norm_face;
        _norms[i*6 + 2] = norm_face;
        _norms[i*6 + 3] = norm_face;
        _norms[i*6 + 4] = norm_face;
        _norms[i*6 + 5] = norm_face;
    } 
    //
    // for(int i = 0; i < 36; ++i) {
    //     std::cout << _norms[i] << std::endl;
    // }

    glBindBuffer(GL_ARRAY_BUFFER, norm_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_norms),
                 &_norms[0][0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sh_cube::compute_normals() {
    for(int i = 0; i < 6; ++i) {
        vec4 a = _verts[i*6 + 0]; 
        vec4 c = _verts[i*6 + 1]; 
        vec4 d = _verts[i*6 + 2]; 

        vec4 norm_face = normalize(cross(d-a, c-a)); 
        norm_face.w = 0;

        _norms[i*6 + 0] = norm_face;
        _norms[i*6 + 1] = norm_face;
        _norms[i*6 + 2] = norm_face;
        _norms[i*6 + 3] = norm_face;
        _norms[i*6 + 4] = norm_face;
        _norms[i*6 + 5] = norm_face;
    } 
    //
    // for(int i = 0; i < 36; ++i) {
    //     std::cout << _norms[i] << std::endl;
    // }

    glBindBuffer(GL_ARRAY_BUFFER, norm_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_norms),
                 &_norms[0][0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void sh_cube::set_rotationaxis(vec4 axis) {

    _rotaxis = axis;
    _rotaxis.w = 0;
}

void sh_cube::set_rotationpos(vec4 pos) {
    _rotpos = pos;
}

void sh_cube::rotate(float angle) {
    _transformation = shatranslate(_rotpos)
                     *sharotateav(angle, _rotaxis)
                     *shatranslate(-_rotpos)
                     *_transformation;
}

void sh_cube::render(uint32 vpos, uint32 color,
                     uint32 normal, uint32 model_tran,
                     uint32 texture, uint32 textured_bool_loc) {
    
    glBindBuffer(GL_ARRAY_BUFFER, norm_vbo);
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);    
    glEnableVertexAttribArray(vpos);
    glVertexAttribPointer(vpos, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glEnableVertexAttribArray(color);
    glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, 0);

    if(( texture != 0) && (uv_vbo !=  0)) {
        glUniform1i(textured_bool_loc, 1);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
        glEnableVertexAttribArray(texture);
        glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 0, 0);
    } else {
        glUniform1i(textured_bool_loc, 0);
    }

 
    
    glUniformMatrix4fv(model_tran, 1, GL_TRUE, &_transformation[0][0]); 

    glDrawArrays(GL_TRIANGLES, 0, 36);
 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


vec3 sh_cube::get_size() {
    vec3 size = _scale/2.0;
    size.x += _position.x;
    size.y += _position.y;
    size.z += _position.z;
    return size;
}

void sh_cube::move_to(vec4 position) {
    mat4 temp = shatranslate(position)*shatranslate(-_position);    
    _transformation = temp*_transformation;
    _position = position;
    _rotpos = _rotpos + position;
}


