#version 330 core

smooth in float depth;
//uniform vec3 vecViewA;
smooth in vec3 color;
//in vec3 normal;
//in vec3 lightVec;
out vec4 coloro;


void main(){
    coloro = vec4(color,1.0f);
    gl_FragDepth    = 1.0-depth/10.0;
}
