#include <GL/glew.h> // or #include <gl/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include "util/helper.c"

// Specify a horizontal offset via a uniform and move the triangle to the right side of the screen in the vertex
// shader using this offset value:
#define SHADER_PATH "D:/Opengl_C/Project_1/res/shaders/002exercise2.shader"


void processInput(GLFWwindow * window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
}

int main(int argc, char const *argv[])
{   
    
    if(!glfwInit()){
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL); 

    if(!window){
        printf("Failed to initialize window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    /*
        Create shaders
     */

    ShaderProgram *program = ReadShader(SHADER_PATH);
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vert = program->vertexShaderSource;
    glShaderSource(vertexShader, 1, &vert, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char const *frag = program->fragmentShaderSource;
    glShaderSource(fragmentShader, 1, &frag, NULL);
    glCompileShader(fragmentShader);

    int success;
    char msg[256];

    // check if shader compilation was a success or not
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // if not successful get the error and print it
    if(!success){
        glGetShaderInfoLog(vertexShader, 256, NULL, msg);
        printf("Failed to compile shader: %s\n", msg);
    }

    /*
        Link shaders
    */

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 256, NULL, msg);
        printf("Failed to link shaders: %s", msg);
    }


    float vertices[] = {
        // position, color
        0.5f, -0.5f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,
    };


    // unsigned int vbo[2];
    // glGenBuffers(2, vbo);
    // printf("%d\n", vbo[0]);
    // printf("%d", vbo[1]);
    
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);

    // any buffer calls we make will be used to configure the currently bound buffer i.e. vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // copies user defined data into currently bound buffer i.e. GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell opengl how it should interpret vertex data(per vertex attribute)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
    
    //  set the layout (location=0) in vertex shader to be used there
    glEnableVertexAttribArray(0); 

    // setting openGL to wireframe mode
    if(strstr(argv[1], "GL_LINE")){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }// setting openGL to Fill mode
    else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    while(!glfwWindowShouldClose(window)){

        processInput(window);
        // Set the color to clear the screen
        glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
        // Clears the buffer(previous image drawn on screen)
        glClear(GL_COLOR_BUFFER_BIT);

        int uniformPos = glGetUniformLocation(shaderProgram, "offSet");
        glUseProgram(shaderProgram);
        glUniform3f(uniformPos, 0.7f, 0.0f, 0.0f);

        // Drawing the triangle
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // Swaps buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
    return 0;
}