#include "util/helper.c"

#define SHADER_PATH "D:/Opengl_C/Project_1/res/shaders/006Camera.shader"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow * window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

float fov = 45.0f;
float pitch = 0.0f;
float yaw = -90.0f;    

int firstMouse = 1;

int main(){

    if(!glfwInit()){
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Camera", NULL, NULL);

    if(!window){
        printf("Failed to create window\n");
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glClearColor(0.311f, 0.546f, 0.777f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);


    if(glewInit() != GLEW_OK){
        printf("Failed to initialize OpenGL\n");
        return -1;
    }

    ShaderProgram *shaderSource = ReadShader(SHADER_PATH);
    unsigned int program = CreateShader(shaderSource->vertexShaderSource, shaderSource->fragmentShaderSource);

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

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // int stride = 5*sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int textures[2];
    glGenTextures(2, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(TEX_PATH, &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);


    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

    vec3_t cubePosition[] = {
        vec3( 0.0f,  0.0f,  0.0f), 
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

    mat4_t model = m4_identity();

    // get the direction of the camera
    // vec3_t cameraPos = vec3(0.0f, 0.0f, 3.0f);
    // vec3_t cameraTarget = vec3(0.0f, 0.0f, 0.0f);
    // vec3_t cameraDir = v3_norm(v3_sub(cameraPos, cameraTarget));
    // vec3_t cameraFront = vec3(0.0f, 0.0f, -1.0f);

    // // Defining the x-axis of the camera
    // vec3_t up = vec3(0.0f, 1.0f, 0.0f); // is the rotation of the camera
    // vec3_t cameraRight = v3_norm(v3_cross(up, cameraDir));

    // // Defining the y-axis of the camera
    // vec3_t cameraUp = v3_norm(v3_cross(cameraRight, cameraDir));

    vec3_t cameraPos = vec3(0.0f, 0.0f, 3.0f);
    vec3_t cameraFront = vec3(0.0f, 0.0f, -1.0f);
    vec3_t cameraUp = vec3(0.0f, 1.0f, 0.0f);


    // lookAt 
    mat4_t view = m4_look_at(cameraPos, cameraFront, cameraUp);

    mat4_t perspective = m4_perspective(fov, (float)WIDTH/HEIGHT, 0.1f, 100.0f);

    const float cameraSpeed = 2.5f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    vec3_t direction;


    while(!glfwWindowShouldClose(window)){

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, 1);
        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            cameraPos = v3_add(cameraPos, v3_muls(cameraFront, cameraSpeed*deltaTime));
            // cameraPos.z -= cameraSpeed*deltaTime;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            cameraPos = v3_sub(cameraPos, v3_muls(cameraFront, cameraSpeed*deltaTime));
            // cameraPos.z += cameraSpeed*deltaTime;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            cameraPos = v3_sub(cameraPos, v3_muls(v3_norm(v3_cross(cameraFront, cameraUp)), cameraSpeed*deltaTime));
            // cameraPos.x -= cameraSpeed*deltaTime;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            // cameraPos = v3_add(cameraPos, v3_muls(v3_norm(v3_cross(cameraUp, cameraFront)), cameraSpeed*deltaTime));
            cameraPos = v3_add(cameraPos, v3_muls(v3_norm(v3_cross(cameraFront, cameraUp)), cameraSpeed*deltaTime));
            // cameraPos.x += cameraSpeed*deltaTime;
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);


        glUseProgram(program);
        glBindVertexArray(vao);
        // float camX = sin(time) * 10.0f;
        // float camZ = cos(time) * 10.0f;
        // view = m4_look_at(cameraPos, cameraTarget, up);
        // view = m4_look_at(vec3(camX, 0.0f, camZ), cameraTarget, up);
        direction.x = cos(degToRad(yaw)) * cos(degToRad(pitch));
        direction.y = sin(degToRad(pitch));
        direction.z = sin(degToRad(yaw)) * cos(degToRad(pitch));
        cameraFront = v3_norm(direction);
        view = m4_look_at(cameraPos, v3_add(cameraPos, cameraFront), cameraUp);

        for(int i=0; i < 10; i++){
            
            mat4_t translate = m4_translation(cubePosition[i]);
            mat4_t rotation = m4_rotation(i, vec3(0.1f, 0.5f, 1.0f));
            model = m4_mul(translate, rotation);
            // model = m4_mul(rotation, translate);
            int modelLoc = glGetUniformLocation(program, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m[0]);
            int viewLoc = glGetUniformLocation(program, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.m[0]);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        perspective = m4_perspective(fov, (float)WIDTH/HEIGHT, 0.1f, 100.0f);
        int perspectiveLoc = glGetUniformLocation(program, "perspective");
        glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, perspective.m[0]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow *window, double xPosIn, double yPosIn){

    if(firstMouse){
        lastX = xPosIn;
        lastY = yPosIn;
        firstMouse = 0;
    }
    
    float xoffset = xPosIn - lastX;
    float yoffset = lastY - yPosIn; // reversed since y-coordinates range from bottom to top

    lastX = xPosIn;
    lastY = yPosIn;

    const float sensivity = 0.1f;
    xoffset *= sensivity;
    yoffset *= sensivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f){
        pitch = 89.0f;
    }
    if (pitch < -89.0f){
        pitch = -89.0f;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}