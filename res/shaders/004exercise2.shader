#shader vertex
#version 410 core

layout (location=0) in vec2 position;
layout (location=1) in vec3 color;
layout (location=2) in vec2 tex;

out vec2 fragTex;
uniform mat4 translate;
uniform mat4 scale;

void main(){
    gl_Position = translate * scale * vec4(position, 0.0, 1.0);
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