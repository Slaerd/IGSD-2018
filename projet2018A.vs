#version 330 core

//Entrees
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor_modelspace;
layout(location = 2) in vec3 vertexNormal_modelspace;

// Sortie
smooth out vec3 color;
smooth out float depth;
smooth out vec3 fragPos;
smooth out vec3 normal;

// Params
uniform mat4 modelMatrixA;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(){
  gl_Position = projectionMatrix * viewMatrix * modelMatrixA * vec4(vertexPosition_modelspace, 1.0);
  fragPos     = vec3(modelMatrixA * vec4(vertexPosition_modelspace, 1.0));
  depth       = gl_Position.z;
  color       = vertexColor_modelspace;
  normal = vertexNormal_modelspace;
}
