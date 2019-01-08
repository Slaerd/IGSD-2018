#version 330 core

smooth in float depth;
smooth in float depthOG;
smooth in vec3 color;
smooth in vec3 normal;
out vec4 coloro;
smooth in vec3 lightDir1;
smooth in vec3 lightDir2;
smooth in vec3 lightDir3;
smooth in vec3 lightDir4;

uniform vec3 camPos;

void main(){
    vec3 white = vec3(1.0,1.0,1.0);
    vec3 lightDir;                      //Contiendra les directions de nos lumieres

    vec3 ambient = color * 0.3f; //Composante ambiente assez elevee pour ne pas avoir un resultat trop moche
    vec3 diffuse = vec3(.0,.0,.0);
    vec3 speculaire = vec3(.0,.0,.0);

    vec3 lightPos[4];           //On stocke nos lumieres dans un array pour iterer dessus
    lightPos[0] = lightDir1;
    lightPos[1] = lightDir2;
    lightPos[2] = lightDir3;
    lightPos[3] = lightDir4;

    for(int i = 0; i < 4; i++){
        lightDir = lightPos[i];
        vec3 lightDirNorm = normalize(lightDir);    //Normalisation pour les produits scalaires

        float intensity = max(max(0.1, 0.4/dot(lightDir,lightDir)),min(1.0,0.4/dot(lightDir,lightDir)));

        vec3 normalNorm = normalize(normal);
        vec3 camPosNorm = normalize(camPos);

        vec3 lightReflect = 2 * dot(normalNorm,lightDirNorm) * normalNorm - lightDirNorm; //Vecteur qui dirige la lumiere refletee
        lightReflect = normalize(lightReflect);

        float diffK = max(dot(lightDirNorm,normalNorm),.0);
        diffuse += intensity * color * diffK;

        float specK = max(pow(dot(lightReflect,camPosNorm),21),.0);
        speculaire += intensity * white * specK;
    }

    coloro = vec4(ambient + diffuse + speculaire,1.0f);

    gl_FragDepth    = 1.0-depth/10.0;
}
