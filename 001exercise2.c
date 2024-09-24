// Now create the same 2 triangles using two different VAOs and VBOs for their data
// Solution: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.4.hello_triangle_exercise2/hello_triangle_exercise2.cpp

#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>


int main(int argc, char const *argv[])
{
    
    if(!glfwInit()){
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "Exercise 2", NULL, NULL);

    if(!window){
        printf("Failed to create a window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        printf("Failed to initliaze OpenGL\n");
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

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    float vertices1[] = {
        -0.5f, 0.25f, 0.0f,
        -0.75f, -0.5f, 0.0f,
        -0.25f, -0.5f, 0.0f,
    };

    float vertices2[] ={
        0.5f, 0.25f, 0.0f,
        0.25f, -0.5f, 0.0f,
        0.75f, -0.5f, 0.0f
    };

    // unsigned int vao1;
    // glGenVertexArrays(1, &vao1);
    // glBindVertexArray(vao1);

    // unsigned int vbo1;
    // glGenBuffers(1, &vbo1);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo1);


    unsigned int vao[2], vbo[2];
    glGenVertexArrays(2, vao);
    glGenBuffers(2, vbo);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
    
    glEnableVertexAttribArray(0);

    // unsigned int vao2;
    // glGenVertexArrays(1, &vao2);
    // glBindVertexArray(vao2);

    // unsigned int vbo2;
    // glGenBuffers(1, &vbo2);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);



    while(!glfwWindowShouldClose(window)){

        glClearColor(0.2f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        for(int i=0; i < sizeof(vao)/sizeof(unsigned int); i++){

            glBindVertexArray(vao[i]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

