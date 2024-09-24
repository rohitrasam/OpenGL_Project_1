#shader vertex
#version 410 core

layout (location=0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main(){
    gl_Position = perspective * view * model * vec4(pos, 1.0);
}

#shader fragment
#version 410 core

uniform vec3 lightColor;
out vec4 FragColor;

void main(){
    FragColor = vec4(lightColor, 1.0);
}