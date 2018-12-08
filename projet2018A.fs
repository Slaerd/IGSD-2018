#version 330 core

smooth in float depth;
in vec3 color;
in vec3 normal;
in vec3 lightPos;
out vec4 coloro;

void main(){

  vec3 colorLight = color * -dot(lightPos,normal); //Calcul lumiere diffuse
  //coloro          = vec4(color, 1.0);
  if(dot(lightPos,normal) < 0)                //Si la face est opposee a la lumiere on la colore
    coloro          = vec4(colorLight, 0.0);
  else
    coloro = vec4(0.0,0.0,0.0,0.0); //Sinon prout
  gl_FragDepth    = 1.0-depth/10.0;
}
