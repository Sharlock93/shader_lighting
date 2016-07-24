#define GLEW_STATIC
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Shar\headers\shar.h>
#include <Shar\headers\sharfun.h>
#include <Shar\headers\sharinit.h>
#include <Shar\headers\sh_camera3D.h>

#include <sh_types.h>

#include <sh_diamond_square.h>
#include <sh_bmp_reader.h>

#include <sh_grid.h>
#include <sh_cube.h>
#include <sh_cone.h>
#include <cmath>

using namespace std;

const uint32 v_attrib_loc = 0;
const uint32 c_attrib_loc = 1;
const uint32 n_attrib_loc = 2;

const uint32 model_mat_attrib_loc = 3;
const uint32 proj_mat_attrib_loc = 4;
const uint32 cam_mat_attrib_loc = 5;

const uint32 cam_pos_loc = 6;


GLFWwindow* init();

float vel = 0.1;
float speed = 20;

void cam_keys(GLFWwindow *window, sh_camera3D &cam, sh_camera3D &cam2, double time) {
    static double x = 0;
    static double y = 0;

    glfwGetCursorPos(window, &x, &y);
    vel = time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.move_forward(vel);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cam.move_forward(vel*speed);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        
        cam.move_forward(-vel);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cam.move_forward(-vel*speed);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.move_left(-vel);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cam.move_left(-vel*speed);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        
        cam.move_left(vel);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cam.move_left(vel*speed);
    }
   

    //Todo(sharo): take this into the camera class? maybe?
    // - this needs further explinations, as what these numbers mean.
    cam.increase_pitch((2.0 * y / 500.0 + 1.0)*20);
    cam.increase_yaw  ((2.0 * x / 500.0 - 1.0)*20);
}

struct mou {
    vec4 pos; 
    mou(float x, float y);
    vec4 _points[4];
    vec4 _colors[4];
    uint32 _vbo;
    uint32 _color_vbo;

    void render(uint32 vbo, uint32 color);
    void update_pos(float x, float y);
};

void mou::update_pos(float x, float y) {
    vec4 diff = vec4(x, y, 0, 1); 
    _points[0] = vec4(-10,  10, 0, 1) + diff;
    _points[1] = vec4( 10,  10, 0, 1) + diff;
    _points[2] = vec4( 10, -10, 0, 1) + diff;
    _points[3] = vec4(-10, -10, 0, 1) + diff;
    pos = diff;
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*4, &_points[0], GL_DYNAMIC_DRAW);
}

mou::mou(float x, float y) {
    pos = vec4(x, y, 0, 1);
    
    _points[0] = vec4(-5,  5, 0, 1) + pos;
    _points[1] = vec4( 5,  5, 0, 1) + pos;
    _points[2] = vec4( 5, -5, 0, 1) + pos;
    _points[3] = vec4(-5, -5, 0, 1) + pos;

 
    _colors[0] = vec4(1, 0, 0, 1);
    _colors[1] = vec4(1, 0, 0, 1);
    _colors[2] = vec4(1, 0, 0, 1);
    _colors[3] = vec4(1, 0, 0, 1);


    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*4, _points, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*4, _colors, GL_DYNAMIC_DRAW);
}

void mou::render(uint32 vbo, uint32 color) {
   
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableVertexAttribArray(vbo); 
    glVertexAttribPointer(vbo, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
    glEnableVertexAttribArray(color); 
    glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

struct draw_vec {
    draw_vec(vec4 vec, vec4 origin);
    vec4 v;
    vec4 o;
    vec4 vecs[4];
    uint32 vbo;
    void update_vec(vec4 vec, vec4 origin);
    void update_vec_per(vec4 vec, vec4 origin, float t);
    void render(uint32 vbo_attrib, uint32 color_attrib);
};

draw_vec::draw_vec(vec4 vec, vec4 origin) {
    v = normalize(vec);
    o = origin;
    vecs[0] = o;
    vecs[1] = o + v*100;
    vecs[2] = vec4(1, 0, 0, 1);
    vecs[3] = vec4(1, 0, 0, 1);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*4, &vecs[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_vec::render(uint32 vbo_attrib, uint32 color_attrib) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vbo_attrib);
    glEnableVertexAttribArray(color_attrib);
    glVertexAttribPointer(vbo_attrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, 0, (void *)( sizeof(vec4)*2 ));

    glDrawArrays(GL_LINES, 0, 2);
}

void draw_vec::update_vec(vec4 vec, vec4 origin) {
    v = normalize(vec);
    o = origin;
    vecs[0] = o;
    vecs[1] = o + v*100;
    vecs[2] = vec4(1, 0, 0, 1);
    vecs[3] = vec4(1, 0, 0, 1);

    vecs[0].w = 1;
    vecs[1].w = 1;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*4, &vecs[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}

void draw_vec::update_vec_per(vec4 vec, vec4 origin, float t) {
    v = normalize(vec);
    o = origin;
    vecs[0] = o;
    vecs[1] = o + v*t;
    vecs[2] = vec4(1, 0, 0, 1);
    vecs[3] = vec4(1, 0, 0, 1);

    vecs[0].w = 1;
    vecs[1].w = 1;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*4, &vecs[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}


struct rect_2D_draw {
    rect_2D_draw(vec4 start, vec4 end);
    uint32 vbo;
    vec4 _start;
    vec4 _end;
    vec4 vecs[8];

    void update_vec(vec4 start, vec4 end);
    void render(uint32 vbo_attrib, uint32 color_attrib);
};



rect_2D_draw::rect_2D_draw(vec4 start, vec4 end) {
    _start = start;
    _end = end;

    if(_start.x == 0) {

        vecs[0] = _start;
        vecs[1] = vec4(0, _start.y, _end.z, 1);
        vecs[2] = _end;
        vecs[3] = vec4(0, _end.y, _start.z, 1);

    } else if(_start.y == 0) {

        vecs[0] = _start;
        vecs[1] = vec4(_start.x, 0, _end.z, 1);
        vecs[2] = _end;
        vecs[3] = vec4(_end.x  , 0, _start.z, 1);
    }
    else if(_start.z == 0) {  
        vecs[0] = _start;
        vecs[1] = vec4( _start.x, _end.y, 0, 1);
        vecs[2] = _end;
        vecs[3] = vec4(_end.x,  _start.y, 0, 1);
    }

    vecs[4] = vec4(1, 0, 0, 1);
    vecs[5] = vec4(1, 0, 1, 1);
    vecs[6] = vec4(0, 0, 1, 1);
    vecs[7] = vec4(0, 1, 1, 1);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*8, vecs, GL_DYNAMIC_DRAW);
}

void rect_2D_draw::update_vec(vec4 start, vec4 end) {
    _start = start;
    _end = end;

    if(_start.x == 0) {

        vecs[0] = _start;
        vecs[1] = vec4(0, _start.y, _end.z, 1);
        vecs[2] = _end;
        vecs[3] = vec4(0, _end.y, _start.z, 1);

    } else if(_start.y == 0) {

        vecs[0] = _start;
        vecs[1] = vec4(_start.x, 0, _end.z, 1);
        vecs[2] = _end;
        vecs[3] = vec4(_end.x  , 0, _start.z, 1);
    }
    else if(_start.z == 0) {  
        vecs[0] = _start;
        vecs[1] = vec4( _start.x, _end.y, 0, 1);
        vecs[2] = _end;
        vecs[3] = vec4(_end.x,  _start.y, 0, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*8, vecs, GL_DYNAMIC_DRAW);
}

void rect_2D_draw::render(uint32 vbo_attrib, uint32 color_attrib) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vbo_attrib);
    glEnableVertexAttribArray(color_attrib);
    glVertexAttribPointer(vbo_attrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, 0, (void *)( sizeof(vec4)*4 ));

    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

int main(int argc, char ** argv) {
    GLFWwindow *window = init();
 
    glfwSwapInterval(0);
    sh_camera3D cam(vec4(0, 0, 0, 1), vec4(0, 0, -1, 1), vec4(0, 1, 0, 1));
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    sh_grid t(100, 100, 1);
    double x = 0;
    double y = 0;
    glfwGetCursorPos(window, &x, &y);
    mou mouse_pointer(x, y);
  
    glfwSetTime(0);    
    double time1 = glfwGetTime();
    double time2 = glfwGetTime();

    double dt = 1.0/60.0;
    double accumlator = 0.0;

    mat4 orthographics = shaortho(-250, 250, 250 , -250, -1, 1);
    mat4 perspective = shaperspective(45.0f, 1, 0.1f, 1000.0);
    std::cout << perspective << std::endl;

    vec4 directions = cam.get_matrix().z;

    draw_vec vecor(vec4(0, 1, 0, 1), vec4(0, 0, 0, 1));

    vec4 mouse_screen_coord;
    // glfwGetCursorPos(window, &mouse_x, &mouse_y);

    mouse_screen_coord.x = 0;
    mouse_screen_coord.y = 0;
 
    vec4 mouse_coord_eye;
    vec4 mouse_final;
    bool update = false;

    int start_drawing = 0;
    int inits = 0;

    vec4 first_point;
    vec4 second_point;

    vec4 plane_normal = vec4(0, 0, 1, 0);

    rect_2D_draw makers_wish(vec4(0, 5, 5, 1), vec4(0, 10, 10, 1));

    while (!glfwWindowShouldClose(window)) { 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        while(accumlator >= dt) {
            cam_keys(window, cam, cam, dt);
            
            accumlator -= dt; 

        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        
        if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            update = !update;
        }

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            start_drawing = 1;
        }

        directions = normalize(cam.get_matrix().z); 
        double cos_d_t = dot(-directions, plane_normal);
        double up_origin = dot(cam.get_position(), plane_normal);
        double t_is_boop = -up_origin/cos_d_t;

        vecor.update_vec_per(-directions, cam.get_position(), t_is_boop);

        if(vecor.vecs[1].z != 0) {
            std::cout <<  "hello "<< std::endl;
        }

        


        if(start_drawing) {
            if(!inits) {
                first_point = vecor.vecs[1];
                inits = 1;
            }

            second_point = vecor.vecs[1]; 

            makers_wish.update_vec(first_point, second_point);
        }

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
            start_drawing = 0;
            inits = 0;
        }

        // std::cerr << "0: " << makers_wish.vecs[0]  << std::endl;
        // std::cerr << "1: " << makers_wish.vecs[1]  << std::endl;

        vec4 cam_pos = cam.get_position();
        glUniformMatrix4fv(cam_mat_attrib_loc, 1, GL_TRUE, cam.get_matrix());


        glUniform4fv(cam_pos_loc, 1, &cam_pos[0]);        

        glUniformMatrix4fv(proj_mat_attrib_loc, 1, GL_TRUE, perspective);
        t.render(v_attrib_loc, c_attrib_loc, n_attrib_loc, model_mat_attrib_loc, 8, 7);
        vecor.render(v_attrib_loc, c_attrib_loc);

        makers_wish.render(v_attrib_loc, c_attrib_loc);

        glUniformMatrix4fv(cam_mat_attrib_loc, 1, GL_TRUE, mat4(1));
        glUniformMatrix4fv(proj_mat_attrib_loc, 1, GL_TRUE, orthographics);
        mouse_pointer.render(v_attrib_loc, c_attrib_loc);


        glfwSwapBuffers(window);

        time2 = time1;
        time1 = glfwGetTime();
        accumlator += ( time1-time2 );
        glfwPollEvents();
    }


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

    // float width = 1366;
    // float height = 768;

    float width = 500;
    float height = 500;

    // window = glfwCreateWindow(width, height, "Lighting", glfwGetPrimaryMonitor(), nullptr);
    window = glfwCreateWindow(width, height, "Lighting", nullptr, nullptr);

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


    glUniformMatrix4fv(proj_mat_attrib_loc, 1, GL_TRUE, shaperspective(45.0f, width/height, 0.1f, 1000.0));
    // glUniformMatrix4fv(2, 1, GL_TRUE, shaortho(-100.0f, 100.0f, 100.0f, -100.0, -100, 100));
    return window;
}
