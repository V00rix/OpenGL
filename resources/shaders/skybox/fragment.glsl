#version 400 core
out vec4 color;
in vec3 textureCoords;

uniform samplerCube skybox;

void main(){
    color = texture(skybox, textureCoords);
}
