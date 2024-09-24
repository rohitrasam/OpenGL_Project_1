#include <GL/glew.h>    // include before GLFW
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600
#define BUFFER_SIZE 200
#define SHADER_PATH "D:/Opengl_C/Project_1/res/shaders/basic.shader"
// #define SHADER_PATH "../res/shaders/basic.shader" // D:/Opengl_C/Project_1/res/shaders/basic.shader


typedef struct{

    char VertexSource[BUFFER_SIZE];
    char FragmentSource[BUFFER_SIZE];

} ShaderProgramSource;

static ShaderProgramSource *ParseShader(char *filepath){
    
    FILE *fptr = fopen(filepath, "r");

    enum ShaderType {
            NONE=-1, 
            VERTEX=0, 
            FRAG=1
        };

    char line[BUFFER_SIZE];
    char **shaders = malloc(sizeof(char *) * 2);

    int type = NONE;
    char *curr_line = fgets(line, BUFFER_SIZE, fptr);

    while(curr_line){
        if(strstr(line, "#shader"))
        {
            if(strstr(line, "vertex")){
                type = VERTEX;
            }
            else if(strstr(line, "fragment")){
                type = FRAG;
            }
            shaders[type] = malloc(BUFFER_SIZE);
            strcpy(shaders[type], "");
        }
        else{
            strcat(shaders[type], line);
        }
        curr_line = fgets(line, BUFFER_SIZE, fptr);
    }

    fclose(fptr);

    ShaderProgramSource *sps = malloc(sizeof(ShaderProgramSource));
    strcpy(sps->VertexSource, shaders[0]);
    strcpy(sps->FragmentSource, shaders[1]);

    return sps;
}

static unsigned int CompileShader(unsigned int type, const char source[])
{
    unsigned int id = glCreateShader(type);
    const char* src = &source[0]; // == source; memory address of the 1st character of the string
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        printf("\nFailed to compile %s shader!", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
        printf("\n%s", message);
        glDeleteShader(id);
        return 0;
    }
    
    return id;

}

static unsigned int CreateShader(const char vertexShader[], const char fragmentShader[])
{

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, 1);
    }
}


int main()
{
    if(!glfwInit())
        return -1;

    // Don't include next 3 lines, GLEW throws error
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // with this ue glGenVertexArrays(), glBindVertextArray()
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learning GL", NULL, NULL);

    if(!window){
        printf("Failed to create window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Always initialize after glfwMakeContextCurrent()
    if(glewInit() != GLEW_OK){
        printf("Error initializing GLEW\n");
        return -1; 
    }

    // glViewport(0, 0, 800, 600);
    // glClearColor(0.2f, 0.4f, 0.7f, 1.0f);

    float vertices[] = {
        0.5f, -0.5f,    // 0
       -0.5f, -0.5f,    // 1
        -0.5f, 0.5f,    // 2
        0.5f, 0.5f,     // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    }; // has to be unsigned

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer;
    glGenBuffers(1, &buffer); // -> id of the generated buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // binding buffer to GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copying postions into the actual buffer

    // Enable that vertex attribute i.e 0
    glEnableVertexAttribArray(0);
    // starting index, component count of each vertex, dtype, normalized?, amount of bytes bewteen each vertex or, steps to the next component/vertex(stride), pointer? 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0); // enables us to access the vaiable inside shader also links the `buffer` with the `vao`

    // float position is passed to vertex shader from main.c by using VertexAttribPointer
    // location=0 -> means position var is located at index 0(1st param of VertexAttribPointer)


    unsigned int ibo;
    glGenBuffers(1, &ibo); // -> id of the generated ibo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // binding buffer to GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),  indices, GL_STATIC_DRAW); // copying postions into the actual buffer

    
    ShaderProgramSource *source = ParseShader(SHADER_PATH);
    unsigned int shader = CreateShader(source->VertexSource, source->FragmentSource);
    glUseProgram(shader);

    // uniformss are set per draw
    int location = glGetUniformLocation(shader, "u_color"); // u_color is in shader file of type uniform
    glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);

    // Shader is a block of program/block of strings that runs on GPU(not CPU)
    // 1. Vetrex Shaders gets called 3 times
    // 2. Fragment/Pixel Shaders - runs once for each pixel that needs to get rasterized(draw on screen)
    // Many more....

    // Next 4 lines unbound everything
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    

    float r = 0.0f;
    float increment = 0.01;
    // glClearColor(0.4, 0.1, 0.2, 1.0);
    while(!glfwWindowShouldClose(window)){

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        // with CORE we don't need the next 3 lines
        // glBindBuffer(GL_ARRAY_BUFFER, buffer);
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    

        if(r > 1.0f){
            increment = -0.01f;
        }
        else if(r < 0.0f){
            increment = 0.01f;
        }

        r += increment;
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays(GL_TRIANGLES, 0, 4);
        // shape to draw, number of indices, dtype(always unsigned it)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);


        
        // swap the 2 buffers: front & back
        glfwSwapBuffers(window);
        
        // poll for process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
