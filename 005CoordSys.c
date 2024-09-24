// #define STB_IMAGE_IMPLEMENTATION
// #define MATH_3D_IMPLEMENTATION
// #include "stb_image_loader/stb_image.h"
// #include "math3d/math_3d.h"
#include "util/helper.c"
// #include <GL/glew.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <GLFW/glfw3.h>
// #include <math.h>

#define SHADER_PATH "D:/Opengl_C/Project_1/res/shaders/005CoordSys.shader"

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

    glfwSwapInterval(1);
    glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glViewport(0, 0, WIDTH, HEIGHT);

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

    // float vertices[] = {
    // //   position,      color,             texture
    //      0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    //      0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    //     -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    //     -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f
    // };

    // float vertices[] = {
    // //   position,          color,            texture
    //      0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    //      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    // };

    // unsigned int indices[] = {
    //     // front
    //     0, 1, 2,
    //     0, 2, 3,

    // };

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(6*sizeof(float)));
    // glEnableVertexAttribArray(2);

    // glGenBuffers(1, &ebo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

    // float view[] = {
    //     1.0f, 0.0f,  0.0f, 0.0f,
    //     0.0f, 1.0f,  0.0f, 0.0f,
    //     0.0f, 0.0f,  1.0f, 0.0f,
    //     0.0f, 0.0f,  -4.0f, 1.0f,
    // };

    // float model2[] = {
    //     1.0f, 0.0f, 0.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f, 0.0f,
    //     0.0f, 0.0f, 1.0f, 0.0f,
    //     0.0f, 0.0f, 0.0f, 1.0f,
    // };

    // float projection[] = {
    //     1/((float)(WIDTH/(float)HEIGHT)*tan(degToRad(30))),                0.0f, 0.0f,  0.0f,
    //                                                   0.0f, 1/tan(degToRad(30)), 0.0f,  0.0f,
    //                                                   0.0f,                0.0f, 1.0f,  1.0f,
    //                                                   0.0f,                0.0f, 0.0f,  1.0f,
    // };

    // column major
    // float projection[] = {
    //     2.41f, 0.0f, 0.0f, 0.0f,
    //     0.0f, 2.41f, 0.0f, 0.0f,
    //     0.0f, 0.0f, -1.0f, -1.0f,
    //     0.0f, 0.0f, -0.2f, 0.0f,
    // };

    mat4_t view = m4_translation(vec3(0.0f, 0.0f, -4.0f));

    mat4_t projection = m4_perspective(90, (float)(WIDTH/HEIGHT), 0.1f, 100.f);

    mat4_t model2 = m4_rotation(0.4, vec3(0.0f, 1.0f, 0.2f));

    // m4_print(model2);

    // prints matrix row major wise but stores column major wise ✅✅
    // |   2.41   0.00   0.00   0.00 |
    // |   0.00   2.41   0.00   0.00 |
    // |   0.00   0.00  -1.00  -0.20 |
    // |   0.00   0.00  -1.00   0.00 |
    // m4_print(projection);

    vec3_t cubePosition[] = {
        vec3( 0.0f,  0.0f, -3.0f),
        vec3( 2.0f,  5.0f, -15.0f),
        vec3(-1.5f, -2.2f, -2.5f),
        vec3(-3.8f, -2.0f, -12.3f),
        vec3( 2.4f, -0.4f, -3.5f),
        vec3(-1.7f,  3.0f, -7.5f),
        vec3( 1.3f, -2.0f, -2.5f),
        vec3( 1.5f,  2.0f, -2.5f),
        vec3( 1.5f,  0.2f, -1.5f),
        vec3(-1.3f,  1.0f, -1.5f)
    };

    while(!glfwWindowShouldClose(window)){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

           if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
                view.m[3][2] -= 0.1;
                printf("%f\n", view.m[3][2]);
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
                view.m[3][2] += 0.1;
                printf("%f\n", view.m[3][2]);
            }

        float time = glfwGetTime();
        // model2[0] = cos(time);
        // model2[2] = sin(time);
        // model2[8] = -sin(time);
        // model2[10] = cos(time);

        glUseProgram(program);
        glBindVertexArray(vao);

        for(unsigned int i=0; i<10; i++){

            mat4_t model2 = m4_rotation(i*0.04, vec3(0.0f, 1.0f, 0.2f));
            if(i % 3 == 0){
                model2 = m4_rotation(time, vec3(0.0f, 1.0f, 0.2f));
            }
            mat4_t view = m4_translation(cubePosition[i]);
            int rotationLoc2 = glGetUniformLocation(program, "model2");
            glUniformMatrix4fv(rotationLoc2, 1, GL_FALSE, model2.m[0]);
            int translationLoc = glGetUniformLocation(program, "view");
            glUniformMatrix4fv(translationLoc, 1, GL_FALSE, view.m[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        int projectionLoc = glGetUniformLocation(program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.m[0]);
        // glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, projection);

        // glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
    return 0;
}