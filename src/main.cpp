#define GLEW_STATIC
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Shar\headers\shar.h>
#include <Shar\headers\sharfun.h>
#include <Shar\headers\sharinit.h>
#include <Shar\headers\sh_camera3D.h>

#include <stdio.h>
#include <stdint.h>

#include <sh_material.h>
#include <sh_diamond_square.h>
#include <sh_bmp_reader.h>

#include <grid.h>
#include <cmath>

using namespace std;


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
float speed = 10;

glm::vec3 eye(0, 10, 10);
glm::vec3 at(0, 0, 0);
glm::vec3 up(0, 1, 0);

float field_view = 0;

void cam_keys(GLFWwindow *window, sh_camera3D &cam, sh_camera3D &cam2) {
    static double x = 0;
    static double y = 0;

    glfwGetCursorPos(window, &x, &y);

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


int main(int argc, char ** argv) {
    GLFWwindow *window = init();
 
    sh_camera3D cam(vec4(0, 10, 10, 1), vec4(0, 0, 0, 1), vec4(0, 1, 0, 1));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

     
    int detail = 4;
    int num = pow(2, detail) + 1;
    //
    float *height_map = (float*) malloc(sizeof(float)*num*num);
    

    float *height_temp = (float *) malloc(sizeof(float)*num*num);

    // diamond_square( height_map, num-1, 1, 20);
    grid t(50, 50, num-1);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
    // vec3 *p = t.get_points();
    
    while (!glfwWindowShouldClose(window)) { 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        cam_keys(window, cam, cam);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        sh_bmp_img img = sh_read_bmp("test.bmp"); 

        for(int i = 0; i < img.height; ++i) {
            int index_s = i*img.width;
            for(int j = 0; j < img.width; ++j) { 
                float point_s = (img.data[index_s + j].red/255.0f);
                height_temp[index_s + j] =  point_s*20.0;
            } 
        }

        delete[] img.orig_file_ptr; 
   
        t.seed_height_map(height_temp);    

        glUniformMatrix4fv(3, 1, GL_TRUE, cam.get_matrix());

        t.render();
        glfwSwapBuffers(window);
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
