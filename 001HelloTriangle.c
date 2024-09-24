#include <GL/glew.h> // or #include <gl/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600


void processInput(GLFWwindow * window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
}

int main(){

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

    const char *vertexShaderSource = 
    "#version 410 core\n"
    "layout (location=0) in vec3 aPos;\n"
    "layout (location=1) in vec3 aColor;\n"
    "out vec3 fragColor;\n"
    "void main(){\n"
    "gl_Position = vec4(aPos, 1.0f);\n"
    "fragColor = aColor;\n"
    "}\n";

    const char *fragmentShaderSource = "#version 410 core\n"
    "in vec3 fragColor;\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "FragColor = vec4(fragColor, 1.0f);\n"
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
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 3, 2
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0);
    
    //  set the layout (location=0) in vertex shader to be used there
    glEnableVertexAttribArray(0); 

    // Tell opengl how it should interpret vertex data(per vertex attribute)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)12);
    
    //  set the layout (location=1) in vertex shader to be used there
    glEnableVertexAttribArray(1); 

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glUseProgram(shaderProgram);

    // setting openGL to wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // setting openGL to Fill mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while(!glfwWindowShouldClose(window)){

        processInput(window);
        // Set the color to clear the screen
        glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
        // Clears the buffer(previous image drawn on screen)
        glClear(GL_COLOR_BUFFER_BIT);

        // Drawing the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // used with ebo
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swaps buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
    return 0;
}