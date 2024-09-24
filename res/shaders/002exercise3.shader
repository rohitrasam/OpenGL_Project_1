#shader vertex
#version 410 core

layout (location=0) in vec3 pos;
out vec3 aPos;
void main(){

    gl_Position = vec4(pos, 1.0);
    aPos = pos;
}

#shader fragment
#version 410 core
in vec3 aPos;
out vec4 FragColor;

void main(){
    FragColor = vec4(aPos, 1.0);
}
