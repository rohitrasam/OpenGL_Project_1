#shader vertex
#version 410 core

layout (location=0) in vec3 position;
layout (location=1) in vec2 tex;

out vec2 fragTex;
uniform mat4 projection;
// uniform mat4 model1;
uniform mat4 model2;
uniform mat4 view;


void main(){
    gl_Position = projection * view * model2 * vec4(position, 1.0);
    fragTex = tex;
}

#shader fragment
#version 410 core

in vec2 fragTex;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
    FragColor = mix(texture(texture1, fragTex), texture(texture2, fragTex), 0.2);
}