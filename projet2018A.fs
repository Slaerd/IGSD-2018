#version 330 core

smooth in float depth;
smooth in vec3 color;
smooth in vec3 normal;
out vec4 coloro;
in vec3 lightPosNorm;

uniform vec3 camPos;

void main(){
    vec3 white = vec3(1.0,1.0,1.0);
    vec3 lightPos = vec3(1.0f,.5f,.0f);                           // 1 est la valeur max en x et en z de nos courbes
    lightPos = normalize(lightPos);                                 //et y depend de notre increment qu'on recupere dans le buffer
    vec3 normalNorm = normalize(normal);
    vec3 camPosNorm = normalize(camPos);

    vec3 lightReflect = 2 * dot(normalNorm,lightPos) * normalNorm - lightPos; //Vecteur qui dirige la lumiere refletee

    //vec3 lightReflect = (-camPosNorm - lightPos)/2.0f;
    lightReflect = normalize(lightReflect);

    vec3 ambient = color * 0.2f;
    vec3 diffuse = vec3(1.0,1.0,1.0);
    vec3 speculaire = vec3(1.0,1.0,1.0);

    float diffK = max(dot(lightPos,normalNorm),.0);
    diffuse = color * diffK*0.8;

    float specK = max(pow(dot(lightReflect,camPosNorm),1),.0);
    speculaire = white * specK;

    coloro = vec4(ambient + diffuse + speculaire,1.0f);
    //coloro = vec4(color,1.0f);
    gl_FragDepth    = 1.0-depth/10.0;
}
