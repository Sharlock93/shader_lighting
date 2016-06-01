#define GLEW_STATIC
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Shar\headers\shar.h>
#include <Shar\headers\sharfun.h>
#include <Shar\headers\sharinit.h>
#include <Shar\headers\sh_camera3D.h>

#include <sh_types.h>

#include <stdio.h>
#include <stdint.h>

#include <sh_material.h>
#include <sh_diamond_square.h>
#include <sh_bmp_reader.h>

#include <grid.h>
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


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GLFWwindow* init();

float vel = 0.1;
float speed = 20;

glm::vec3 eye(0, 10, 10);
glm::vec3 at(0, 0, 0);
glm::vec3 up(0, 1, 0);

float field_view = 0;

mat4 ratio_scale;

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
    cam.increase_pitch((-2.0 * y / 500.0 + 1.0)*20);
    cam.increase_yaw  ( -(2.0 * x / 500.0 - 1.0)*20);
}

struct sh_tree {
    sh_tree();
    sh_cone first_part;
    sh_cone second_part;
    sh_cone third_part;

    sh_cube trunk;

    vec4 _pos;

    void render(uint32 vpos, uint32 color,
                uint32 normal, uint32 model_tran,
                uint32 texture, uint32 textured_bool_loc);

    void move_to(vec4 pos);
};

sh_tree::sh_tree() {
    _pos = vec4(0, 0, 0, 1);
    first_part = sh_cone(8, 1, 1);
    second_part = sh_cone(8, 1, 1);
    third_part = sh_cone(8, 1, 1);

    trunk.compute_normals();

    first_part.compute_normals();
    second_part.compute_normals();
    third_part.compute_normals(); 

    trunk.set_size(0.2, 2, 0.2);

    vec4 temp[36];
    for(int i = 0; i < 36; ++i) {
        temp[i] = vec4(0.77, 135/255, 0, 1);
    } 

    trunk.colors(temp);

    vec4 *cone_color = new vec4[first_part._number_points];
    for(int i = 0; i < first_part._number_points; ++i) {
        cone_color[i] = vec4(0, 0.8, 0, 1);
    }

    first_part.colors(cone_color);
    second_part.colors(cone_color);
    third_part.colors(cone_color);

    first_part.move_to(vec4(0, 2, 0, 1));
    second_part.move_to(vec4(0, 2.8, 0, 1));
    third_part.move_to(vec4(0, 3.6, 0, 1));

}

void sh_tree::move_to(vec4 pos) {
    mat4 teraman = shatranslate(pos)*shatranslate(-_pos);
    first_part._transformation = teraman*first_part._transformation; 

    second_part._transformation = teraman*second_part._transformation; 

    third_part._transformation = teraman*third_part._transformation; 

    trunk._transformation = teraman*trunk._transformation; 
    _pos = pos;
}

void sh_tree::render(uint32 vpos, uint32 color,
                uint32 normal, uint32 model_tran,
                uint32 texture, uint32 textured_bool_loc) {

    first_part.render(vpos, color, normal,
                      model_tran, texture, textured_bool_loc);

    second_part.render(vpos, color, normal,
                      model_tran, texture, textured_bool_loc);

    third_part.render(vpos, color, normal,
                      model_tran, texture, textured_bool_loc);

    trunk.render(vpos, color, normal,
                      model_tran, texture, textured_bool_loc);
}

struct sh_cat {
    sh_cat();
    sh_cube front_left;
    sh_cube front_right;  
    sh_cube back_left;
    sh_cube back_right;  
    sh_cube body;
    sh_cube head;
    sh_cube bbox;

    mat4 _transformation;
    vec4 _position;
    vec4 _forward_dir;

    void render(uint32 vpos, uint32 color,
                uint32 normal, uint32 model_tran,
                uint32 texture, uint32 textured_bool_loc);
    void move_legs(float angle);
    void move_cat(vec4 pos, float angle);
    void rotate_cat(float angle);
    void move_forward(float velocity, float angle);
}; 

sh_cat::sh_cat() {
    vec4 color[36];
    for(int i = 0; i < 36; ++i) {
        color[i] = vec4(1, 1, 1, 1);
    }

    _position = vec4(0, 0, 0, 1);
    _forward_dir = vec4(1, 0, 0, 0);

    front_left.compute_normals();
    front_right.compute_normals();  
    back_left.compute_normals();
    back_right.compute_normals();  
    body.compute_normals();
    head.compute_normals();

    bbox.compute_normals();
    bbox.colors(color);
    bbox.set_size(2, 2, 2);

    body.colors(color);
    head.colors(color);
    front_left.colors(color);
    front_right.colors(color);
    back_right.colors(color);
    back_left.colors(color);

    head.set_size(1, 1, 1);
    body.set_size(2, 1, 1);
    head.move_to(vec4(3, 2, 0, 1));

    front_left.set_size(2/5.0, 1, 2/5.0);
    front_right.set_size(2/5.0, 1, 2/5.0);
    front_left.move_to(vec4(7/5.0, -2, -3/5.0, 1));
    front_right.move_to(vec4(7/5.0, -2,  3/5.0, 1));

    back_left.set_size(2/5.0, 1, 2/5.0);
    back_right.set_size(2/5.0, 1, 2/5.0);
    back_left.move_to(vec4(-7/5.0, -2, -3/5.0, 1));
    back_right.move_to(vec4(-7/5.0, -2,  3/5.0, 1));

    front_left.set_rotationpos(vec4(7/5.0, -1, -3/5.0, 1)); 
    front_left.set_rotationaxis(vec4(0, 0, 1, 1));

    front_right.set_rotationpos(vec4(7/5.0, -1, 3/5.0, 1)); 
    front_right.set_rotationaxis(vec4(0, 0, 1, 1));
    
    back_right.set_rotationpos(vec4(-7/5.0, -1, +3/5.0, 1)); 
    back_right.set_rotationaxis(vec4(0, 0, 1, 1));

    back_left.set_rotationpos(vec4(-7/5.0, -1, -3/5.0, 1)); 
    back_left.set_rotationaxis(vec4(0, 0, 1, 1));
}

void sh_cat::render(uint32 vpos, uint32 color,
                    uint32 normal, uint32 model_tran,
                    uint32 texture, uint32 textured_bool_loc)
{
    front_left.render(vpos, color, normal, model_tran, texture, textured_bool_loc);
    front_right.render(vpos, color, normal, model_tran, texture, textured_bool_loc);  
    back_left.render(vpos, color, normal, model_tran, texture, textured_bool_loc);
    back_right.render(vpos, color, normal, model_tran, texture, textured_bool_loc);  
    body.render(vpos, color, normal, model_tran, texture, textured_bool_loc);
    head.render(vpos, color, normal, model_tran, texture, textured_bool_loc);
    bbox.render(vpos, color, normal, model_tran, texture, textured_bool_loc);
}

void sh_cat::move_legs(float angle) {
    front_left.rotate(angle); 
    front_right.rotate(-angle); 

    back_left.rotate(angle); 
    back_right.rotate(-angle); 
}

void sh_cat::move_cat(vec4 pos, float angle) { 
    mat4 teraman = shatranslate(pos)*shatranslate(-_position);

    front_left._transformation = teraman*front_left._transformation;
    front_right._transformation = teraman*front_right._transformation;
    back_left._transformation = teraman*back_left._transformation;
    back_right._transformation = teraman*back_right._transformation;
    body._transformation = teraman*body._transformation;
    head._transformation = teraman*head._transformation;
    bbox._transformation = teraman*bbox._transformation;
    bbox._position = pos;
    _position = pos;

    front_right._rotpos = teraman*front_right._rotpos;
    front_left._rotpos = teraman*front_left._rotpos;
    back_left._rotpos = teraman*back_left._rotpos;
    back_right._rotpos = teraman*back_right._rotpos;
    head._rotpos = teraman*head._rotpos;
    body._rotpos = teraman*body._rotpos;


    move_legs(angle);
}

void sh_cat::move_forward(float velocity, float angle) {
    mat4 teraman = shatranslate(_forward_dir*velocity);
    _position = teraman*_position;

    front_left._transformation = teraman*front_left._transformation;
    front_right._transformation = teraman*front_right._transformation;
    back_left._transformation = teraman*back_left._transformation;
    back_right._transformation = teraman*back_right._transformation;
    body._transformation = teraman*body._transformation;
    head._transformation = teraman*head._transformation;
    bbox._transformation = teraman*bbox._transformation;
    bbox._position = teraman*bbox._position;

    front_right._rotpos = teraman*front_right._rotpos;
    front_left._rotpos = teraman*front_left._rotpos;
    back_left._rotpos = teraman*back_left._rotpos;
    back_right._rotpos = teraman*back_right._rotpos;
    head._rotpos = teraman*head._rotpos;
    body._rotpos = teraman*body._rotpos;


    move_legs(angle);
}

void sh_cat::rotate_cat(float angle) {
    mat4 teraman = shatranslate(_position)*sharotatey(angle)*shatranslate(-_position);
    _forward_dir = teraman*_forward_dir;
    _position = teraman*_position;

    front_left._transformation = teraman*front_left._transformation;
    front_right._transformation = teraman*front_right._transformation;
    back_left._transformation = teraman*back_left._transformation;
    back_right._transformation = teraman*back_right._transformation;
    body._transformation = teraman*body._transformation;
    head._transformation = teraman*head._transformation;
    
    front_left._rotaxis = teraman*front_left._rotaxis;
    front_right._rotaxis = teraman*front_right._rotaxis;
    back_left._rotaxis = teraman*back_left._rotaxis;
    back_right._rotaxis = teraman*back_right._rotaxis;
    body._rotaxis = teraman*body._rotaxis;
    head._rotaxis = teraman*head._rotaxis;
    
    front_right._rotpos = teraman*front_right._rotpos;
    front_left._rotpos = teraman*front_left._rotpos;
    back_left._rotpos = teraman*back_left._rotpos;
    back_right._rotpos = teraman*back_right._rotpos;
    head._rotpos = teraman*head._rotpos;
    body._rotpos = teraman*body._rotpos;
    // _position = pos;
}

bool check_cube_col(sh_cube *ac, sh_cube *bc) {
    vec3 a = ac->get_size();
    vec3 b = bc->get_size();
    
   //check the X axis
   if(abs(a.x - b.x) < ( ac->_scale.x + bc->_scale.x ))
   {
      //check the Y axis
      if(abs(a.y - b.y) < ( ac->_scale.y + bc->_scale.y ))
      {
          //check the Z axis
          if(abs(a.z - b.z) < ( ac->_scale.z + bc->_scale.z ))
          {
             return true;
          }
      }
   }

   return false;
}

bool check_grid_cube(grid *g, sh_cube *read) {
    vec3 size_r = read->get_size();
    vec4 gd = g->_position;
    for(int i = 0; i < g->_num_points; ++i) {
        if(g->_points[i].y < 10) continue;

        if((( size_r.x  ) > ( g->_points[i].x + gd.x )) &&
           ((read->_position.x-read->_scale.x/2.0 ) < ( g->_points[i].x + gd.x ) )) {
            if(( ( size_r.z  ) > ( g->_points[i].z + gd.z) )
                    && ( (read->_position.z-read->_scale.z/2.0 ) < ( g->_points[i].z + gd.z ) )) {
                return true;
            }
        }
    }

    return false; 
}

void load_level(grid *g, int level, int num) {
    string maze_ = string("maze") + std::to_string(level) + string(".bmp");
    std::cout << level << std::endl;
    sh_bmp_img img = sh_read_bmp(maze_.c_str()); 
   

    float *height_temp = (float *) malloc(sizeof(float)*num*num);

    for(int i = 0; i < img.height; ++i) {
        int index_s = i*img.width;
        for(int j = 0; j < img.width; ++j) { 
            float point_s = (img.data[index_s + j].red/255.0f);
            height_temp[index_s + j] =  point_s*15;
        } 
    }

    g->seed_height_map(height_temp);
    g->compute_normals(); 
    g->test_compute_color();
}

int main(int argc, char ** argv) {
    GLFWwindow *window = init();
 
    glfwSwapInterval(0);
    sh_camera3D cam(vec4(90, 200, 0, 1), vec4(100, 0, 0, 1), vec4(1, 0, 0, 1));
    sh_camera3D cam2(vec4(90, 200, 0, 1), vec4(100, 0, 0, 1), vec4(1, 0, 0, 1));
    sh_camera3D cam3(vec4(90, 200, 0, 1), vec4(100, 0, 0, 1), vec4(1, 0, 0, 1));
    //
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    //
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //
    sh_cube cube;
    // cube.move_to(vec4(-17.8154,9.15531,29.513,1));

    cube.set_size(2, 5, 2);
    cube.compute_normals();
    cube.read_texture("box.bmp", 0, 0, 32);
    cube.set_rotationpos(vec4(0, 10, 0, 1));
    cube.set_rotationaxis(vec4(1, 0, 0, 0));
    cube.move_to(vec4(0, 10, 0, 1));
    // //
    sh_cube cube2;
    
    cube2.compute_normals();
    cube2.set_size(2, 5, 2);
    cube2.move_to(vec4(4, 5, 0, 1));
    cube2.set_rotationaxis(vec4(1, 0, 0, 0));
    //
    for(int i = 0; i < 36; ++i) {
        std::cout << cube._uv[i]  << std::endl;
    }
    // cube.set_size(5, 5, 5);
    vec4 temp[36];
    for(int i = 0; i < 36; ++i) {
        temp[i] = vec4(cos(cube._verts[i].x), sin(cube._verts[i].y), 1, 1);
    }

    cube.colors(temp);
    cube2.colors(temp);

    int detail = 8;
    int num = pow(2, detail) + 1;

    // float *height_map = (float*) malloc(sizeof(float)*num*num);
    float *height_temp = (float *) malloc(sizeof(float)*num*num);

    // diamond_square( height_map, num-1, 1, 100);
    grid t(100, 100, num-1);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //
    grid maze(100, 100, num-1);

    maze.set_position(vec4(100, 10, 0,1));

    
    glfwSetTime(0);    
    double time1 = glfwGetTime();
    double time2 = glfwGetTime();
    //
    sh_bmp_img img = sh_read_bmp("test1.bmp"); 

    for(int i = 0; i < img.height; ++i) {
        int index_s = i*img.width;
        for(int j = 0; j < img.width; ++j) { 
            float point_s = (img.data[index_s + j].red/255.0f);
            height_temp[index_s + j] =  point_s*20;
        } 
    }

    t.seed_height_map(height_temp);    

    

    // delete[] img.orig_file_ptr; 


    t.compute_normals();   // vec3 *p = t.get_points();
    t.test_compute_color();

    grid t2(100, 100, 8);
    t2.compute_normals();
    t2.test_compute_color();
    t2.set_position(vec4(0, 6.5, 0, 1));
    vec4 *colors = new vec4[t2._drawpoint];
    for(int i = 0; i < t2._drawpoint; ++i) {
        colors[i] = vec4(0, 0, 0.5, 1);
    }
    t2.colors(colors, t2._drawpoint);


    sh_cone cone(8, 1, 5);
    cone.compute_normals();
    vec4 *cone_color = new vec4[cone._number_points];
    for(int i = 0; i < cone._number_points; ++i) {
        cone_color[i] = vec4(0, 0.8, 0, 1);
    }

    cone.colors(cone_color);

    sh_tree trees[15];


    trees[0].move_to(vec4(47.3992,12.1178,38.7131,1));
    trees[1].move_to(vec4(38.0659,12.0312,34.2657,1));
    trees[2].move_to(vec4(22.5132,12.196,25.5247,1));
    trees[3].move_to(vec4(-0.513706,12.4458,14.735,1));
    trees[4].move_to(vec4(-20.2366,11.7447,-0.356307,1));
    trees[5].move_to(vec4(-36.451,12.253,-19.1764,1));
    trees[6].move_to(vec4(-44.8934,12.078,-38.7266,1));
    trees[7].move_to(vec4(-33.6999,12.6896,-48.2569,1));
    trees[8].move_to(vec4(-29.7152,11.7252,-37.0691,1));
    trees[9].move_to(vec4(-24.4366,11.9654,-26.3462,1));

    trees[10].move_to(vec4(-13.2174,12.5992,-11.7247,1));
    trees[11].move_to(vec4(-3.47907,11.5819,-4.31991,1));
    trees[12].move_to(vec4(7.78948,11.4143,2.39962,1));
    trees[13].move_to(vec4(26.0398,11.4024,10.2626,1));
    trees[14].move_to(vec4(46.6739,10.9944,20.5583,1)) ;


    double dt = 1.0/60.0;
    double accumlator = 0.0;
    
    // float radi = 1;
    // float x = 0;
    // float z = 0;
    int direction = 1;
    int angle_to = 0;

    sh_cat cat;
    vec4 cat_pos(0, 0, 0, 1);
    cat.move_cat(vec4(48.6173,12.85,28.557,1), 0);

    sh_cube houses[11];
    for(int i = 0; i < 11; ++i ) {
        houses[i].compute_normals();
        houses[i].set_size(2*cos(i), 2, 2*sin(i));
        houses[i].compute_normals();
        houses[i].read_texture("box.bmp", 0, 0, 32);
    }

    houses[0].move_to(vec4(44.0564,13.1817,41.2413,1));
    houses[1].move_to(vec4(8.11655,14.7661,30.4794,1));
    houses[2].move_to(vec4(-11.1618,9.33582,29.3249,1));
    houses[3].move_to(vec4(-24.0908,8.87668,38.1554,1));
    houses[4].move_to(vec4(-13.3851,10.1489,14.3803,1));
    houses[5].move_to(vec4(-39.7776,8.67752,-12.3963,1));
    houses[6].move_to(vec4(-48.7776,8.43696,-1.63241,1));
    houses[7].move_to(vec4(12.0271,11.113,-47.1656,1));
    houses[8].move_to(vec4(26.9727,12.046,-45.7317,1));
    houses[9].move_to(vec4(43.5628,12.0497,-44.6181,1));
    houses[10].move_to(vec4(32.9603,12.1047,-25.3969,1));

    sh_cube testa;
    testa.compute_normals();
    testa.colors(temp);
    testa.move_to(vec4(164.671,10,-4.50326,1));

    sh_cube testa2;
    testa2.compute_normals();
    testa2.colors(temp);
    testa2.move_to(vec4(0, 10, 0, 1));
    // testa2.set_size(3, 3, 3);

    vec4 testa2_move(46.6943, 11,35.7598,1);

    std::cout << maze._num_points << std::endl;
    int level = 1;

    bool game_start = 0;

    bool only_once = true;
    double time_passed = 0;

    load_level(&maze, level, num);
    

    while (!glfwWindowShouldClose(window)) { 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        bool dtes = check_grid_cube(&maze, &testa2);


        bool level_end = check_cube_col(&testa, &testa2);

        while(accumlator >= dt) {
            if(!game_start )
                cam_keys(window, cam, cam, dt);

            accumlator -= dt; 
            if(angle_to == 25) direction = -1;
            else if(angle_to == -25) direction = 1;
            angle_to += direction;

            bool dtes = check_grid_cube(&maze, &testa2);

            // std::cout << dtes << std::endl;
            {
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                    if(angle_to == 25) direction = -1;
                    else if(angle_to == -25) direction = 1;
                    angle_to += direction;

                    cat.move_forward(4*dt, direction*40*dt);

                    testa2_move.x += dt*speed;

                    if(dtes == 1) {
                        testa2_move.x -= 2*dt*speed; 
                        
                        dtes = 0;
                    }
                }

                if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                    game_start = true;
                    cam3 = cam;
                    cam = cam2;
                }

                if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
                    game_start = false;
                    cam = cam3;
                    cam3 = cam;
                    
                }

                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                    cat.rotate_cat(-40*dt);

                    testa2_move.z += dt*speed;

                    if(dtes == 1) {
                        testa2_move.z -= 2*dt*speed;
                        dtes = 0;
                    }
                }


                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                    if(angle_to == 25) direction = -1;
                    else if(angle_to == -25) direction = 1; 
                    cat.move_forward(-4*dt, -direction*40*dt); 

                    testa2_move.x -= dt*speed;

                    if(dtes == 1) {
                        testa2_move.x += 2*dt*speed;
                        dtes = 0;
                    }
                }

                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                    cat.rotate_cat(40*dt);
                    testa2_move.z -= dt*speed;

                    if(dtes == 1)
                    {
                        testa2_move.z += 2*dt*speed;
                        dtes = 0; 
                    }
                }

            }
            testa2.move_to(testa2_move);

            if(level_end && only_once) {
                level++;
                load_level(&maze, level, num);
                testa2_move = vec4(46.6943, 10 ,35.7598,1);
                level_end = false;
                only_once = false;
            }


            if (( glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS ) && only_once  ) {
                only_once = false;
                level++;
                load_level(&maze, level, num);
                testa2_move = vec4(46.6943, 10 ,35.7598,1);
                level_end = false;
                only_once = false;
            }



            if (( glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS ) && only_once  ) {
                only_once = false;
                level = 1;
                load_level(&maze, 1, num);
                testa2_move = vec4(46.6943, 10 ,35.7598,1);
                level_end = false;
                only_once = false;
            }

        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if(dtes == 1) {
            testa2_move = vec4(46.6943, 11,35.7598,1);
        }
       
        //
        vec4 cam_pos = cam.get_position();
        glUniformMatrix4fv(cam_mat_attrib_loc, 1, GL_TRUE, cam.get_matrix());
        glUniform4fv(cam_pos_loc, 1, &cam_pos[0]); 

        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            std::cout << cam_pos  << std::endl;
            cam_pos.y = 0;
            maze.set_position(cam_pos);
        }

        // cone.render(v_attrib_loc, c_attrib_loc,
                    // n_attrib_loc, model_mat_attrib_loc, 8, 7 );
        for(int i = 0; i < 15; ++i) { 
            trees[i].render(v_attrib_loc, c_attrib_loc,
                    n_attrib_loc, model_mat_attrib_loc, 8, 7 );
        }

        for(int i = 0; i < 11; ++i) {
            houses[i].render(v_attrib_loc, c_attrib_loc, n_attrib_loc, model_mat_attrib_loc, 8, 7 );
        }

        t.render(v_attrib_loc, c_attrib_loc,
                 n_attrib_loc, model_mat_attrib_loc,
                 8, 7);
        t2.render(v_attrib_loc, c_attrib_loc,
                 n_attrib_loc, model_mat_attrib_loc,
                 8, 7);
        //

        testa2.render(v_attrib_loc, c_attrib_loc,
                 n_attrib_loc, model_mat_attrib_loc,
                 8, 7);
        testa.render(v_attrib_loc, c_attrib_loc,
                 n_attrib_loc, model_mat_attrib_loc,
                 8, 7);
        cube.render(v_attrib_loc, c_attrib_loc,
                    n_attrib_loc, model_mat_attrib_loc, 8, 7 );
        // cube2.render(v_attrib_loc, c_attrib_loc,
        //             n_attrib_loc, model_mat_attrib_loc, 8, 7);
        //
        cat.render(v_attrib_loc, c_attrib_loc,
                    n_attrib_loc, model_mat_attrib_loc, 8, 7 );
        maze.render(v_attrib_loc, c_attrib_loc,
                    n_attrib_loc, model_mat_attrib_loc, 8, 7 );

        glfwSwapBuffers(window);

        if(time_passed > 2) {
            only_once = true;
            time_passed = 0;
        }


        time2 = time1;
        time1 = glfwGetTime();

        accumlator += ( time1-time2 );
        time_passed += (time1 - time2);

        glfwPollEvents();
    }

    std::cout << "hello world" << std::endl;

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

    float width = 1366;
    float height = 768;

    // float width = 500;
    // float height = 500;

    window = glfwCreateWindow(width, height, "Lighting", glfwGetPrimaryMonitor(), nullptr);
    // window = glfwCreateWindow(width, height, "Lighting", nullptr, nullptr);

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
