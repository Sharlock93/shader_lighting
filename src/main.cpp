#define GLEW_STATIC
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Shar\headers\shar.h>
#include <Shar\headers\sharfun.h>
#include <Shar\headers\sharinit.h>
#include <Shar\headers\sh_camera3D.h>
#define _WINDOWS
#include <sh_material.h>
// #include <sh_shpere.h>


#include <grid.h>
#include <cmath>
#include <random>
using namespace std;


void cube();
void cube(int a);
void triangle_each_color(vec4 a, vec4 b, vec4 c, vec4 color_a, vec4 color_b, vec4 color_c);
const GLfloat  DivideByZeroTolerance = GLfloat(1.0e-07);
// ToDo(): highlight ToDo words ?
typedef struct light {
    vec4 amb; //ambient color
    vec4 diff; //diffuse color
    vec4 spec; //specular color

    vec4 pos; //position of the light, if pos.w  = 0, direction.
} light;

struct vertex {
    vec4 vec;
    vec4 color;
    vec4 normal;
};

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

sh_material lamake = {
                    vec4(0.1, 0.1, 0.1, 1), //amb
                    vec4(0.5, 0.5, 0.5, 1), //diffs
                    vec4(1, 1, 1, 1), //spec
                    vec4(0.1, 0.1, 1, 1.0), // emissi
                    100.0
                    }; //shnini
    
light firstLight = 
                {
                    vec4(1, 1, 1, 1), //amb color
                    vec4(1, 1, 1, 1), //diff
                    vec4(1, 1, 1, 1), //spec
                    vec4(2, 2, 2, 1) //pos
                };
   

GLFWwindow* init();

float vel = 0.1;

const int subs = 5;
const int points_per_triangle = 3;
const int triangle_per_div = 4;
const int triangle_per_tertrahadron = 4;

const int cube_div = 6;

const int face = 6;
const int quad_per_face = pow(4, cube_div);
const int triangle_per_quad = 2;

typedef point2 vec2;

point2 buffer[1024]; 

const int draw = ( pow(triangle_per_div, subs)) * points_per_triangle * triangle_per_tertrahadron;

const int cubea = face*quad_per_face*triangle_per_quad*points_per_triangle;
vec4 test[cubea];
vec4 color[cubea];

vec4 normals[cubea];

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

vec4 compnent_wise_product(vec4 a, vec4 b) {
    return vec4(a.x * b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}


void quad_each(vec4 a, vec4 b, vec4 c, vec4 d,
               vec4 color_a, vec4 color_b, vec4 color_c, vec4 color_d,
               vec4 normal_a, vec4 normal_b, vec4 normal_c, vec4 normal_d);

void quad_each(vertex &a, vertex &b, vertex &c, vertex &d);



int screw_index = 0;
int normal_index = 0;

vec4 unit( const vec4& p ) {
    double len = p.x * p.x + p.y * p.y + p.z * p.z;
    vec4 t;
    //if ( len > DivideByZeroTolerance ) {
        t = p / sqrt(len);
        t.w = 1.0;
    //}
    
    return t;
}

void quad_div(vec4 a, vec4 b, vec4 c, vec4 d, int num_div) {
    if(num_div > 0) {
        vec4 ab = unit(a + b);
        vec4 bc = unit(b + c);
        vec4 cd = unit(c + d);
        vec4 ad = unit(a + d);
        vec4 bd = unit(b + d);
    
        quad_div(unit(a),  unit(ab), unit(bd), unit(ad), num_div-1);
        quad_div(unit(b),  unit(bc), unit(bd), unit(ab), num_div-1);
        quad_div(unit(bd), unit(bc), unit(c ), unit(cd), num_div-1);
        quad_div(unit(bd), unit(cd), unit(d ), unit(ad), num_div-1);

    } else {
        vec4 norm = unit(cross(a-c, a-b)); 
        vertex a_v{unit(a), vec4(1, 1, 1, 1),norm};
        vertex b_v{unit(b), vec4(1, 1, 1, 1),norm};
        vertex c_v{unit(c), vec4(1, 1, 1, 1),norm};
        vertex d_v{unit(d), vec4(1, 1, 1, 1),norm};
        //
        // vertex a_v{unit(a), vec4(1, 1, 1, 1), (a)};
        // vertex b_v{unit(b), vec4(1, 1, 1, 1), (b)};
        // vertex c_v{unit(c), vec4(1, 1, 1, 1), (c)};
        // vertex d_v{unit(d), vec4(1, 1, 1, 1), (d)};
        //
        quad_each(a_v, b_v, c_v, d_v);    

    }
}

void cube(int a) {
    quad_div(v[0], v[1], v[2], v[3], a); //face 1
    quad_div(v[1], v[5], v[6], v[2], a); //face 2
    quad_div(v[5], v[4], v[7], v[6], a); //face 3
    quad_div(v[4], v[0], v[3], v[7], a); //face 4
    quad_div(v[1], v[0], v[4], v[5], a); //face 5
    quad_div(v[3], v[2], v[6], v[7], a); //face 6
}

glm::vec3 eye(0, 10, 10);
glm::vec3 at(0, 0, 0);
glm::vec3 up(0, 1, 0);

float field_view = 0;

float zoom = 0;

void cam_keys(GLFWwindow *window, sh_camera3D &cam, sh_camera3D &cam2) {
    static double x = 0;
    static double y = 0;

    glfwGetCursorPos(window, &x, &y);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        cam.move_forward(vel);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cam.move_forward(vel*5);
        zoom += 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        
        cam.move_forward(-vel);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cam.move_forward(-vel*5);
        zoom -= 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        cam.move_left(-vel);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cam.move_left(-vel*5);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        
        cam.move_left(vel);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cam.move_left(vel*5);
    }
    
        // std::cout << cam.get_position() << std::endl;
    cam.increase_pitch((-2.0 * y / 500.0 + 1.0)*20);
    cam.increase_yaw  ( -(2.0 * x / 500.0 - 1.0)*20);
}

void cam_key(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        zoom += 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { 
        zoom -= 0.1;
    }
}


float t = 0;

vec4 cube_buffer[1024];

int cuber_index = 0;

void quad_make(vec4 a, vec4 b, vec4 c, vec4 d) {
    cube_buffer[cuber_index++] = a;
    cube_buffer[cuber_index++] = b;
    cube_buffer[cuber_index++] = c;

    cube_buffer[cuber_index++] = a;
    cube_buffer[cuber_index++] = c;
    cube_buffer[cuber_index++] = d;
}

void make_cube() {
    quad_make(v[0], v[1], v[2], v[3]);
    quad_make(v[1], v[5], v[6], v[2]); 
    quad_make(v[5], v[4], v[7], v[6]);
    quad_make(v[4], v[0], v[3], v[7]); 
    quad_make(v[1], v[0], v[4], v[5]);
    quad_make(v[3], v[2], v[6], v[7]);
}



int main(int argc, char ** argv) {
    GLFWwindow *window = init();

    make_cube();
 
#if 1
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_buffer), &cube_buffer, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);


    sh_camera3D cam(vec4(0, 10, 10, 1), vec4(0, 0, 0, 1), vec4(0, 1, 0, 1));

    // sh_shpere test;
    // cout << cam.get_matrix() << endl;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    int detail = 8;
    int num = pow(2, detail);
    grid t(50, 50, num);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

    // glLineWidth(2.0);
    t.seed_height_map(20);

    // for(int i = 0; i < t.get_point_size(); ++i) {
    //     if(i%( num+1 ) == 0) cout << std::endl << "i: " << setw(2) << i; 
    //     std::cout << " " << setw(10) << t.get_points()[i].y;
    // }

    while (!glfwWindowShouldClose(window)) { 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        

        cam_keys(window, cam, cam);
        glUniformMatrix4fv(3, 1, GL_TRUE, cam.get_matrix());
        // glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
        t.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#endif
    return 0;
}

GLFWwindow* init() {
    GLFWwindow *window;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    window = glfwCreateWindow(500, 500, "Lighting", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    glewInit();

    GLuint vert = shamkshader(GL_VERTEX_SHADER, "vert.glsl");
    GLuint frgt = shamkshader(GL_FRAGMENT_SHADER, "frgt.glsl");

    GLuint prog = shamkprogram(vert, frgt);

    glUseProgram(prog);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    glUniformMatrix4fv(2, 1, GL_TRUE, shaperspective(100.0f, 1.0f, 0.1f, 1000.0));
    // glUniformMatrix4fv(2, 1, GL_TRUE, shaortho(-100.0f, 100.0f, 100.0f, -100.0, -100, 100));
    return window;
}

void quad_each(vertex &a, vertex &b, vertex &c, vertex &d) {
    test[screw_index] = a.vec;
    color[screw_index] = a.color;
    normals[screw_index++] = vec4(a.normal.x, a.normal.y, a.normal.z, 0);

    test[screw_index] = b.vec;
    color[screw_index] = b.color;
    normals[screw_index++] = vec4(b.normal.x, b.normal.y, b.normal.z, 0);

    test[screw_index] = d.vec;
    color[screw_index] = d.color;
    normals[screw_index++] = vec4(d.normal.x, d.normal.y, d.normal.z, 0);
    
    test[screw_index] = b.vec;
    color[screw_index] = b.color;
    normals[screw_index++] = vec4(b.normal.x, b.normal.y, b.normal.z, 0);
   
    test[screw_index] = c.vec;
    color[screw_index] = c.color;
    normals[screw_index++] = vec4(c.normal.x, c.normal.y, b.normal.z, 0);

    test[screw_index] = d.vec;
    color[screw_index] = d.color;
    normals[screw_index++] = vec4(d.normal.x, d.normal.y, d.normal.z, 0);
}




