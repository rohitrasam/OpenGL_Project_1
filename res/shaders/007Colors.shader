#shader vertex
#version 410 core

layout (location=0) in vec3 pos;
layout (location=1) in vec3 norms;

out vec3 Normals;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main(){
    Normals = norms;
    FragPos = vec3(model * vec4(pos, 1.0));
    gl_Position = perspective * view * vec4(FragPos, 1.0);
}

#shader fragment
#version 410 core

in vec3 Normals;
in vec3 FragPos;


uniform vec3 lightPos;
uniform vec3 ordCubeColor;
uniform vec3 lightCubeColor;

out vec4 FragColor;

void main(){

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = lightCubeColor * ambientStrength;

    // diffuse
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightCubeColor;
    
    vec3 result = ordCubeColor * (ambient + diffuse);
    FragColor = vec4(result, 1.0);
}