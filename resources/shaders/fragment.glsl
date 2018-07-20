#version 400 core

in vec2 UV;
in vec3 fragmentColor;
out vec3 color;

uniform sampler2D texture_sampler;

void main(){
  color = texture( texture_sampler, UV ).rgb;
//  color = fragmentColor;
}