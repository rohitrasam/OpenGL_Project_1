#include "util/helper.c"

#define SHADER_PATH_CUBE "D:/Opengl_C/Project_1/res/shaders/007Colors.shader"
#define SHADER_PATH_LIGHT "D:/Opengl_C/Project_1/res/shaders/007ColorsLight.shader"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow * window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

float fov = 45.0f;
float pitch = 0.0f;
float yaw = -90.0f;    

int firstMouse = 1;


int main(int argc, char const *argv[])
{
    if(!glfwInit()){
        printf("Failed to load GLFW\n");
        return -1;
    }   

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Colors", NULL, NULL);

    if(!window){
        printf("Failed to create window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(glewInit() != GLEW_OK){
        printf("Failed to initialize OpenGL\n");
        return -1;
    }

    // load cube shader
    ShaderProgram *shaderSource = ReadShader(SHADER_PATH_CUBE);
    unsigned int cubeProgram = CreateShader(shaderSource->vertexShaderSource, shaderSource->fragmentShaderSource);
    // load lighting shader
    shaderSource = ReadShader(SHADER_PATH_LIGHT);
    unsigned int lightProgram = CreateShader(shaderSource->vertexShaderSource, shaderSource->fragmentShaderSource);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);

    float vertices[] = {
    // position,          normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

    unsigned int vaos[2];
    glGenVertexArrays(2, vaos);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Ordinary Cube
    glBindVertexArray(vaos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)12);
    glEnableVertexAttribArray(1);
    
    // light Cube
    glBindVertexArray(vaos[1]);
    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already 
    // contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
    glEnableVertexAttribArray(0);


    mat4_t model = m4_identity();

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

    vec3_t lightPos = vec3(1.2f, 1.0f, 2.0f);

    float lightColor[] = {1.0f, 1.0f, 1.0f};
    float cubeColor[] = {1.0f, 0.5f, 0.31f};

    while(!glfwWindowShouldClose(window)){
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        direction.x = cos(degToRad(yaw)) * cos(degToRad(pitch));
        direction.y = sin(degToRad(pitch));
        direction.z = sin(degToRad(yaw)) * cos(degToRad(pitch));
        cameraFront = v3_norm(direction);
        view = m4_look_at(cameraPos, v3_add(cameraPos, cameraFront), cameraUp);
        perspective = m4_perspective(fov, (float)WIDTH/HEIGHT, 0.1f, 100.0f);


        // drawing the odinary Cube
        glUseProgram(cubeProgram);
        glBindVertexArray(vaos[0]);
        int lightPosLoc = glGetUniformLocation(cubeProgram, "lightPos");
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        int ordCubeColorLoc = glGetUniformLocation(cubeProgram, "ordCubeColor");
        glUniform3fv(ordCubeColorLoc, 1, cubeColor);
        int lightCubeColorLoc = glGetUniformLocation(cubeProgram, "lightCubeColor");
        glUniform3fv(lightCubeColorLoc, 1, lightColor);
        model = m4_identity();
        int modelLoc = glGetUniformLocation(cubeProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m[0]);
        int viewLoc = glGetUniformLocation(cubeProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.m[0]);
        int perpectiveLoc = glGetUniformLocation(cubeProgram, "perspective");
        glUniformMatrix4fv(perpectiveLoc, 1, GL_FALSE, perspective.m[0]);

        glDrawArrays(GL_TRIANGLES, 0, 36);


        // drawing the light Cube
        glUseProgram(lightProgram);
        glBindVertexArray(vaos[1]);
        mat4_t translation = m4_translation(lightPos);
        mat4_t scaling= m4_scaling(vec3(0.2f, 0.2f, 0.2f));
        model = m4_mul(translation, scaling);

        modelLoc = glGetUniformLocation(lightProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m[0]);
        viewLoc = glGetUniformLocation(lightProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.m[0]);
        perpectiveLoc = glGetUniformLocation(lightProgram, "perspective");
        glUniformMatrix4fv(perpectiveLoc, 1, GL_FALSE, perspective.m[0]);
        lightCubeColorLoc = glGetUniformLocation(lightProgram, "lightColor");
        glUniform3fv(lightCubeColorLoc, 1, lightColor);

        glDrawArrays(GL_TRIANGLES, 0, 36);

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