// Try drawing a second container with another call to glDrawElements but place it at a different position using 
// transformations only. Make sure this second container is placed at the top-left of the window and instead of 
// rotating, scale it over time (using the sin function is useful here; note that using sin will cause the object 
// to invert as soon as a negative scale is applied):

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image_loader/stb_image.h"
#include "util/helper.c"

#define SHADER_PATH "D:/Opengl_C/Project_1/res/shaders/004exercise2.shader"
#define TEX_PATH "D:/Opengl_C/Project_1/res/textures/wall_texture.jpg"
#define TEX_PATH1 "D:/Opengl_C/Project_1/res/textures/awesomeface.png"



int main(){
    if(!glfwInit()){
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =glfwCreateWindow(WIDTH, HEIGHT, "Textures", NULL, NULL); 

    if(!window){
        printf("Failed to initialize window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    ShaderProgram *shaderSource = ReadShader(SHADER_PATH);
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexSource = shaderSource->vertexShaderSource;
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char msg[256];

    // check if shader compilation was a success or not
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // if not successful get the error and print it
    if(!success){
        glGetShaderInfoLog(vertexShader, 256, NULL, msg);
        printf("Failed to compile shader: %s\n", msg);
    }


    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentSource = shaderSource->fragmentShaderSource;
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);


    // check if shader compilation was a success or not
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    // if not successful get the error and print it
    if(!success){
        glGetShaderInfoLog(fragmentShader, 256, NULL, msg);
        printf("Failed to compile shader: %s\n", msg);
    }


    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program, 256, NULL, msg);
        printf("Failed to link shaders: %s", msg);
    }

    float vertices[] = {
    //   position,      color,             texture
         0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void *)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load(TEX_PATH, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_set_flip_vertically_on_load(1);
    data = stbi_load(TEX_PATH1, &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);
    

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(program, "texture2"), 1);


    float translate1[] = {
         1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
        -0.4f, 0.3f, 0.0f, 1.0f,
    };

    float scale[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    glfwSwapInterval(1);

    while(!glfwWindowShouldClose(window)){

        glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        float time = glfwGetTime();
        scale[0] = fabs(sin(time));
        scale[5] = fabs(sin(time));

        int scaleLoc = glGetUniformLocation(program, "scale");
        glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, scale);
        int translationLoc = glGetUniformLocation(program, "translate");
        glUniformMatrix4fv(translationLoc, 1, GL_FALSE, translate1);
        glUseProgram(program);
        glBindVertexArray(vao);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
    return 0;
}
