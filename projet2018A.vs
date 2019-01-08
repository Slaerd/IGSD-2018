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
out vec3 lightPosNorm;

// Params
uniform mat4 modelMatrixA;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(){
  //mat4 modelMatrixA = projectionMatrix * viewMatrix * modelMatrixA;

  mat3 normalMatrix = mat3( vec3(modelMatrixA[0][0],modelMatrixA[0][1],modelMatrixA[0][2]),
                            vec3(modelMatrixA[1][0],modelMatrixA[1][1],modelMatrixA[1][2]),
                            vec3(modelMatrixA[2][0],modelMatrixA[2][1],modelMatrixA[2][2]));

  gl_Position = projectionMatrix * viewMatrix * modelMatrixA * vec4(vertexPosition_modelspace, 1.0);
  fragPos     = vec3(modelMatrixA * vec4(vertexPosition_modelspace, 1.0));
  depth       = gl_Position.z;

  color       = vertexColor_modelspace;
  normal = transpose(inverse(normalMatrix)) * vertexNormal_modelspace;

  //lightPosNorm = 
}
