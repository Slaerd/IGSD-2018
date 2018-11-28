#version 330 core

//Entrees
layout(location = 0) in vec3 vertexPosition_modelspace;

// Sortie
smooth out float zt;
smooth out float depth;
smooth out vec3 fragPos;

// Params
uniform mat4 modelMatrixB;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
  gl_Position = projectionMatrix * viewMatrix * modelMatrixB * vec4(vertexPosition_modelspace, 1.0);
  fragPos     = vec3(modelMatrixB * vec4(vertexPosition_modelspace, 1.0));
  depth       = gl_Position.z;
  zt          = vertexPosition_modelspace.z;
}
