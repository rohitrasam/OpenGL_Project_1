#shader vertex
#version 410 core

layout (location=0) in vec3 pos;
uniform vec3 offSet;

void main(){
    gl_Position = vec4(pos.x+offSet.x, pos.yz, 1.0);
}

#shader fragment
#version 410 core
out vec4 FragColor;
void main(){
    FragColor=vec4(1.0, 0.5, 0.0, 1.0);
}