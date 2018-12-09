#version 330 core

//Entrees
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor_modelspace;
layout(location = 2) in vec3 vertexNormal_modelspace;

// Sortie
smooth out vec3 color;
smooth out float depth;
smooth out vec3 fragPos;

// Params
uniform mat4 modelMatrixA;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 vecViewA;

void main(){
  gl_Position = projectionMatrix * viewMatrix * modelMatrixA * vec4(vertexPosition_modelspace, 1.0);
  fragPos     = vec3(modelMatrixA * vec4(vertexPosition_modelspace, 1.0));
  depth       = gl_Position.z;
  color       = vertexColor_modelspace;
  vec3 normal = vertexNormal_modelspace;
	vec3 lightVec = vec3(0.0f,0.0f,-1.0f); // 1 est la valeur max en x et en z de nos courbes
  lightVec = normalize(lightVec);                                 //et y depend de notre increment qu'on recupere dans le buffer

  vec3 viewNorm = normalize(vecViewA);
  vec3 lightReflect = 2 * dot(normal,lightVec) * normal - lightVec; //Vecteur qui dirige la lumiere refletee

  vec3 ambient = color * 0.1f;
  vec3 diffuse = vec3(1.0,1.0,1.0);
  vec3 speculaire = vec3(1.0,1.0,1.0);

  /*if(dot(lightVec,normal) < 0){           //Si la face est opposee a la lumiere on la colore


      }
  else
      diffuse = vec3(0.0,0.0,0.0);*/

      diffuse = color * dot(-lightVec,normal)*5; //Calcul lumiere diffuse
      speculaire = color * dot(lightReflect,viewNorm)*2; //Calcul lumiere speculaire
      color = ambient + diffuse + speculaire;

}
