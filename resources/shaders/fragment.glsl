#version 400 core

in vec3 fragmentColor;

out vec3 color;

uniform vec3 u_color;


void main(){
  color = fragmentColor;
//  color = vec3(1.0f, 0.0f, 0.0f);
}