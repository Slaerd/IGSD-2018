#version 330 core

smooth in float depth;
smooth in float zt;

out vec4 color;

void main(){
  // ...

  if (zt <= 0.1)
    color = vec4(1.0,0.0,0.0,1.0);
  else if (zt <= 0.2)
    color = vec4(0.8,0.0,0.0,1.0);
  else if (zt <= 0.3)
    color = vec4(0.6,0.0,0.0,1.0);
  else if (zt <= 0.4)
    color = vec4(0.4,0.0,0.0,1.0);
  else if (zt <= 0.5)
    color = vec4(0.2,0.0,0.0,1.0);
  else if (zt <= 0.6)
    color = vec4(0.0,0.2,0.0,1.0);
  else if (zt <= 0.7)
    color = vec4(0.0,0.4,0.0,1.0);
  else if (zt <= 0.8)
    color = vec4(0.0,0.6,0.0,1.0);
  else if (zt <= 0.9)
    color = vec4(0.0,0.8,0.0,1.0);
  else
    color = vec4(0.0,1.0,0.0,1.0);
  /**
  if (zt <= 0.5)
	color = vec4(1-2*zt,0.0,0.0,1.0);
  else
	color = vec4(0.0, (zt-0.5)*2, 0.0, 1.0);**/
  gl_FragDepth = 1.0-depth/10.0;
}
