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
    vec3 lightDir;

    vec3 ambient = color * 0.2f;
    vec3 diffuse = vec3(.0,.0,.0);
    vec3 speculaire = vec3(.0,.0,.0);

    /*if(depthOG == -2.0){
        lightDir = lightDir1;
    }else if(depthOG == -1.0){
        lightDir = lightDir2;
    }else if(depthOG == .0){
        lightDir = lightDir3;
    }else{
        lightDir = lightDir4;
    }*/
    vec3 lightPos[4];
    lightPos[0] = lightDir1;
    lightPos[1] = lightDir2;
    lightPos[2] = lightDir3;
    lightPos[3] = lightDir4;

    for(int i = 0; i < 4; i++){
        lightDir = lightPos[i];
        vec3 lightDirNorm = normalize(lightDir);

        //vec3 spotMainDir = normalize(vec3(1.0,0.0,1.0));
        float spotParam = 1;
        float distanceParam = min(max(0.01, 0.5/dot(lightDir,lightDir)),min(1.0,0.5/dot(lightDir,lightDir)));

        /*if(dot(lightDirNorm,spotMainDir) > 0)
            spotParam = 1;
        else
            spotParam = 0;*/

        float intensity = spotParam * distanceParam;
        //vec3 lightDirNorm = vec3(1.0,.0,.0);

        vec3 normalNorm = normalize(normal);
        vec3 camPosNorm = normalize(camPos);

        vec3 lightReflect = 2 * dot(normalNorm,lightDirNorm) * normalNorm - lightDirNorm; //Vecteur qui dirige la lumiere refletee
        lightReflect = normalize(lightReflect);

        //float diffK = max(dot(lightDirNorm,normalNorm),.0);
        float diffK = dot(lightDirNorm,normalNorm);
        diffuse += intensity * color * diffK;

        float specK = max(pow(dot(lightReflect,camPosNorm),201),.0);
        speculaire += intensity * white * specK;
    }

    coloro = vec4(diffuse,1.0f);

    gl_FragDepth    = 1.0-depth/10.0;
}
