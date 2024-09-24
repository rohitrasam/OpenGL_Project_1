// Try to draw 2 triangles next to each other using glDrawArrays by adding
//  more vertices to your data
// soution: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.3.hello_triangle_exercise1/hello_triangle_exercise1.cpp



#include <gl/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>


int main(int argc, char const *argv[])
{
    if(!glfwInit()){
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Exercise 1", NULL, NULL);

    if(!window){
        printf("Failed to initialize window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        printf("Failed to initialize OpenGL\n");
        return -1;
    }


    const char *vertexShaderSource = 
    "#version 410 core\n"
    "layout (location=0) in vec3 aPos;\n"
    "void main(){\n"
    "gl_Position = vec4(aPos, 1.0f);\n"
    "}\n";

    const char *fragmentShaderSource = "#version 410 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "FragColor = vec4(1.0f, 0.2f, 0.2f, 1.0f);\n"
    "}";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 256, NULL, msg);
        printf("Failed to link shaders: %s", msg);
    }

    float vertices[] = {
        -0.5f, 0.25f, 0.0f,
        -0.75f, -0.5f, 0.0f,
        -0.25f, -0.5f, 0.0f,

        0.5f, 0.25f, 0.0f,
        0.25f, -0.5f, 0.0f,
        0.75f, -0.5f, 0.0f
    };


    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); 


    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    while(!glfwWindowShouldClose(window)){

        glClearColor(0.2f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao); 
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();

    return 0;
}
