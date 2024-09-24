#shader vertex
#version 410 core
layout (location=0) in vec3 position;

void main(){

    gl_Position = vec4(position, 1.0f);

}

#shader fragment
#version 410 core
out vec4 FragColor;
uniform vec4 ourColor;

void main(){
    FragColor = ourColor;
}