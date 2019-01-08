#version 330 core

//Entrees
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor_modelspace;
layout(location = 2) in vec3 vertexNormal_modelspace;

// Sortie
smooth out vec3 color;
smooth out float depth;
smooth out float depthOG;
smooth out vec3 fragPos;
smooth out vec3 normal;
smooth out vec3 lightDir1;
smooth out vec3 lightDir2;
smooth out vec3 lightDir3;
smooth out vec3 lightDir4;

// Params
uniform mat4 modelMatrixA;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

void main(){

  gl_Position = projectionMatrix * viewMatrix * modelMatrixA * vec4(vertexPosition_modelspace,1.0);
  fragPos     = vec3(modelMatrixA * vec4(vertexPosition_modelspace, 1.0));
  depth       = gl_Position.z;

  color       = vertexColor_modelspace;
  normal = vertexNormal_modelspace;

  depthOG = vertexPosition_modelspace.y;
  vec3 lightPos1 = vec3(.2,-2.0,1.2);
  vec3 lightPos2 = vec3(.2,-1.0,1.2);
  vec3 lightPos3 = vec3(.2,.0,1.2);
  vec3 lightPos4 = vec3(.2,1.0,1.2);
  lightDir1 = lightPos1 - vertexPosition_modelspace;
  lightDir2 = lightPos2 - vertexPosition_modelspace;
  lightDir3 = lightPos3 - vertexPosition_modelspace;
  lightDir4 = lightPos4 - vertexPosition_modelspace;

}
