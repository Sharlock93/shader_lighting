#include <sh_shpere.h>
#include <iostream>
#include <cmath>
using namespace std;

struct vertex {
    vec4 vec;
    vec4 color;
    vec4 normal;
};

vec4 unit( const vec4& p );

void quad_each(vec4 *test, vertex &a, vertex &b, vertex &c, vertex &d, int &screw_index) {
    test[screw_index++] = a.vec;
    test[screw_index++] = b.vec;
    test[screw_index++] = d.vec;
    
    test[screw_index++] = b.vec;
    test[screw_index++] = c.vec;
    test[screw_index++] = d.vec;
}

void quad_div(vec4 *test, vec4 a, vec4 b, vec4 c, vec4 d, int num_div, int &screw_index) {
    if(num_div > 0) {
        vec4 ab = unit(a + b);
        vec4 bc = unit(b + c);
        vec4 cd = unit(c + d);
        vec4 ad = unit(a + d);
        vec4 bd = unit(b + d);
    
        quad_div(test, unit(a),  unit(ab), unit(bd), unit(ad), num_div-1, screw_index);
        quad_div(test, unit(b),  unit(bc), unit(bd), unit(ab), num_div-1, screw_index);
        quad_div(test, unit(bd), unit(bc), unit(c ), unit(cd), num_div-1, screw_index);
        quad_div(test, unit(bd), unit(cd), unit(d ), unit(ad), num_div-1, screw_index);

    } else {
        vec4 norm = unit(cross(a-c, a-b)); 
        // vertex a_v{unit(a), vec4(1, 1, 1, 1),norm};
        // vertex b_v{unit(b), vec4(1, 1, 1, 1),norm};
        // vertex c_v{unit(c), vec4(1, 1, 1, 1),norm};
        // vertex d_v{unit(d), vec4(1, 1, 1, 1),norm};
        //
        vertex a_v{unit(a), vec4(1, 1, 1, 1), unit(a)};
        vertex b_v{unit(b), vec4(1, 1, 1, 1), unit(b)};
        vertex c_v{unit(c), vec4(1, 1, 1, 1), unit(c)};
        vertex d_v{unit(d), vec4(1, 1, 1, 1), unit(d)};
        //
        quad_each(test, a_v, b_v, c_v, d_v, screw_index);    
    }
}


void cube(vec4 *test, int a, int &screw_index) {
    vec4 v[8] = {
             vec4(-1,  1, 1, 1.0),
             vec4( 1,  1, 1, 1.0),
             vec4( 1, -1, 1, 1.0),
             vec4(-1, -1, 1, 1.0),

             vec4(-1,  1, -1, 1.0),
             vec4( 1,  1, -1, 1.0),
             vec4( 1, -1, -1, 1.0),
             vec4(-1, -1, -1, 1.0)
            };

    quad_div(test, v[0], v[1], v[2], v[3], a, screw_index); //face 1
    quad_div(test, v[1], v[5], v[6], v[2], a, screw_index); //face 2
    quad_div(test, v[5], v[4], v[7], v[6], a, screw_index); //face 3
    quad_div(test, v[4], v[0], v[3], v[7], a, screw_index); //face 4
    quad_div(test, v[1], v[0], v[4], v[5], a, screw_index); //face 5
    quad_div(test, v[3], v[2], v[6], v[7], a, screw_index); //face 6
}



sh_shpere::sh_shpere() {
    int face = 6;
    int quad_per_face = pow(4, 0);
    int triangle_per_quad = 2;
    int cubea = face*quad_per_face*triangle_per_quad*3;
    _size = cubea;
    _points = new vec4[cubea];
    _index = 0;
    cube(_points, 0, _index);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_points), _points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void sh_shpere::render() {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, _size);
}

sh_shpere::~sh_shpere() {
    cout << "hello " <<endl;
    delete [] _points;
}
