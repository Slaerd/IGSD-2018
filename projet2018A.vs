#version 330 core

//Entrees
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor_modelspace;
layout(location = 2) in vec3 vertexNormal_modelspace;

// Sortie
out vec3  color;
smooth out float depth;
smooth out vec3 fragPos;
out vec3 normal;
out vec3 lightPos;

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
	lightPos = vec3(1.0f,int(vertexPosition_modelspace.x)/1,1.0f); // 1 est la valeur max en x et en z de nos courbes
  lightPos = normalize(lightPos);                                 //et y depend de notre increment qu'on recupere dans le buffer
}
