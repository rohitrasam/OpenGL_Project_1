#define STB_IMAGE_IMPLEMENTATION
#define MATH_3D_IMPLEMENTATION
#include "../stb_image_loader/stb_image.h"
#include "../math3d/math_3d.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TEX_PATH "D:/Opengl_C/Project_1/res/textures/wooden_texture.jpg"
#define TEX_PATH1 "D:/Opengl_C/Project_1/res/textures/awesomeface.png"
#define PI M_PI
#define degToRad(angleInDeg) ((angleInDeg) * PI / 180)
#define radToDeg(angleInRad) ((angleInRad) * 180/ PI)
#define BUFFER 2048
#define WIDTH 1920
#define HEIGHT 1080


typedef struct{
    char *vertexShaderSource;
    char *fragmentShaderSource;
} ShaderProgram;


ShaderProgram *ReadShader(char *filepath){
    FILE *fptr = fopen(filepath, "r");

    enum ShaderType {
        NONE=-1,
        VERTEX=0,
        FRAG=1
    };
    // an array of pointers that point to char pointer
    char **shaders = malloc(sizeof(char *)*2);
    char line[BUFFER];

    int shaderType = NONE;
    char *curr_line = fgets(line, BUFFER, fptr);

    while(curr_line){
        if(strstr(line, "#shader")){
            if(strstr(line, "vertex")){
                shaderType = VERTEX;
            }
            else if(strstr(line, "fragment")){
                shaderType = FRAG;
            }
            shaders[shaderType] = malloc(BUFFER);
            strcpy(shaders[shaderType], "");
        }
        else{
            strcat(shaders[shaderType], line);
        }
        curr_line = fgets(line, BUFFER, fptr);
    }
    // printf("%s\n%s", shaders[0], shaders[1]);
    fclose(fptr);

    ShaderProgram *shaderProgram = malloc(sizeof(ShaderProgram));
    // strcpy(shaderProgram->vertexShaderSource, shaders[0]);
    // strcpy(shaderProgram->fragmentShaderSource, shaders[1]);
    shaderProgram->vertexShaderSource = shaders[0];
    shaderProgram->fragmentShaderSource = shaders[1];

    return shaderProgram;
}


unsigned int CompileShader(unsigned int shaderType, const char *source){

    int success;
    unsigned int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &source, NULL);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if(!success){
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        char msg[length];
        glGetShaderInfoLog(shaderId, 256, NULL, msg);
        printf("Failed to compile %s shader: %s\n", (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment"), msg);
        return 0;
    }

    return shaderId;
}


unsigned int CreateShader(const char *vertexSource, const char *fragmentSource){
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // glValidateProgram(program);

    // glGetProgramiv(program, GL_LINK_STATUS, &success);
    // if(!success){
    //     glGetProgramInfoLog(program, 256, NULL, msg);
    //     printf("Failed to link shaders: %s", msg);
    // }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

