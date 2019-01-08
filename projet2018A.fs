#version 330 core

smooth in float depth;
smooth in vec3 color;
smooth in vec3 normal;
out vec4 coloro;
smooth in vec3 lightDir;

uniform vec3 camPos;

void main(){
    vec3 white = vec3(1.0,1.0,1.0);

    vec3 lightDirNorm = normalize(lightDir);

    float spotParam = 0;
    float distanceParam = max(min(0.1, 1/dot(lightDir,lightDir)),min(1.0,1/dot(lightDir,lightDir)));

    if(dot(lightDirNorm,vec3(.0,-1.0,.0)) > 0)
        spotParam = 1;
    else
        spotParam = 0;

    float intensity = spotParam * distanceParam;
    //vec3 lightDirNorm = vec3(1.0,.0,.0);

    vec3 normalNorm = normalize(normal);
    vec3 camPosNorm = normalize(camPos);

    vec3 lightReflect = 2 * dot(normalNorm,lightDirNorm) * normalNorm - lightDirNorm; //Vecteur qui dirige la lumiere refletee
    lightReflect = normalize(lightReflect);

    vec3 ambient = color * 0.2f;
    vec3 diffuse = vec3(1.0,1.0,1.0);
    vec3 speculaire = vec3(1.0,1.0,1.0);

    float diffK = max(dot(lightDirNorm,normalNorm),.0);
    diffuse = intensity * color * diffK;

    float specK = max(pow(dot(lightReflect,camPosNorm),201),.0);
    speculaire = intensity * white * specK;

    coloro = vec4(diffuse + speculaire,1.0f);

    gl_FragDepth    = 1.0-depth/10.0;
}
