#version 330 core

smooth in float depth;
smooth in vec3 color;

out vec4 coloro;

void main(){
  //...
  coloro          = vec4(color, 1.0);
  gl_FragDepth    = 1.0-depth/10.0;
}
