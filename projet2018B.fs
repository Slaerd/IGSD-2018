#version 330 core

smooth in float depth;
smooth in float zt;

out vec4 color;

void main(){
  // ...
  color = vec4(0.0, zt, 0.0, 1.0);
  gl_FragDepth = 1.0-depth/10.0;
}
