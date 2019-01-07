#version 330 core

smooth in float depth;
smooth in vec3 color;
smooth in vec3 normal;
out vec4 coloro;

uniform vec3 camPos;

void main(){
    vec3 lightPos = vec3(0.0f,0.0f,1.0f);                           // 1 est la valeur max en x et en z de nos courbes
    lightPos = normalize(lightPos);                                 //et y depend de notre increment qu'on recupere dans le buffer
    vec3 normalNorm = normalize(normal);
    vec3 camPosNorm = normalize(camPos);

    vec3 lightReflect = 2 * dot(normalNorm,lightPos) * normalNorm - lightPos; //Vecteur qui dirige la lumiere refletee

    //vec3 lightReflect = (-camPosNorm - lightPos)/2.0f;
    lightReflect = normalize(lightReflect);

    vec3 ambient = color * 0.3f;
    vec3 diffuse = vec3(1.0,1.0,1.0);
    vec3 speculaire = vec3(1.0,1.0,1.0);

    if(dot(lightPos,normalNorm) > 0)                    //Si la normale est opposee a la lumiere on la colore
        diffuse = color * dot(lightPos,normalNorm);
    else
        diffuse = vec3(0.0,0.0,0.0);

    if(dot(lightReflect,camPosNorm) > 0)
        speculaire = color * dot(lightReflect,camPosNorm);
    else
        speculaire = vec3(0.0,0.0,0.0);

    coloro = vec4(ambient + diffuse + speculaire,1.0f);
    //coloro = vec4(.0,.0,.0,1.0f);
    gl_FragDepth    = 1.0-depth/10.0;
}
