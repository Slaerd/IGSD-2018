// sous mac
// g++ -I/usr/local/include/ -lglfw -lGLEW projet2018.cpp -framework OpenGL -oprojet2018
// ./projet2018

// sous linux
// g++ -I/usr/local/include/ -I/public/ig/glm/ -c projet2018.cpp  -oprojet2018.o
// g++ -I/usr/local projet2018.o -lglfw  -lGLEW  -lGL  -oprojet2018
// ./projet2018

// Inclut les en-têtes standards
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
using namespace std;

#include <stdlib.h>
#include <string.h>
//Care because of my pc
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

float ptime = 0.0f;
float vv = 1.5f;

float nbBands = 8.0f;

int N = 4;

int profondeur_A = 0;
int profondeur_B = 0;

int SMOOTHING_VALS = 1;
int SMOOTHING_VOLS = 1;

// gauss function
float gaussian( float x, float mu, float sigma ) {
  const float a = ( x - mu ) / sigma;
  return std::exp( -0.5 * a * a );
}


// compute a 1D kernel of gauss numbers
vector<float> computeGaussianKernel(int kernelRadius){
  vector<float> kernel(kernelRadius*2);

  //TO DO

  return kernel;
}


vector<float> smoothData(vector<float> &data, vector<float> &kernel){
  vector<float> smoothdata(data.size());
  vector<float> sums(data.size());

  // TO DO

  return smoothdata;
}


void loadData(string filename, vector<float> &vols, vector<float> &vals){
  ifstream file ( filename ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
  string value ;
  getline ( file, value, '\n' ) ; //skip first line

  float minval = INT_MAX;
  float minvol = INT_MAX;
  float maxval = INT_MIN;
  float maxvol = INT_MIN;
	int cpt = 0;
  while ( file.good() ) {
    getline(file,value, ',');
	  if (cpt==5){
			if ( minvol > stof(value)) minvol = stof(value);
			if ( maxvol < stof(value)) maxvol = stof(value);
			//minval = min(minval, stof(value));
			//maxval = max(maxval, stof(value));
			vols.push_back(stof(value));
		} else if (cpt == 6){
			if ( minval > stof(value)) minval = stof(value);
			if ( maxval < stof(value)) maxval = stof(value);
			//minval = min(minvol, stof(value));
			//maxval = max(maxvol, stof(value));
			vals.push_back(stof(value));
			cpt = 0;
		}
		cpt++;
  }
  // on rajoute 2 fausses valeurs a la fin que l'on affichera pas .. mais qu'on utilisera pour normaliser !
  vals.push_back(minval);
  vals.push_back(maxval);
  vols.push_back(minvol);
  vols.push_back(maxvol);
	for (int i = 0; i < vals.size(); i++){
		vals[i] = vals[i]/maxval;
		vols[i] = vols[i]/maxvol;
	}
}


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

  // Create the shaders
  GLuint VertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if(VertexShaderStream.is_open()){
    std::string Line = "";
    while(getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }else{
    printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
    getchar();
    return 0;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if(FragmentShaderStream.is_open()){
    std::string Line = "";
    while(getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;


  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }



  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }



  // Link the program
  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);


  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> ProgramErrorMessage(InfoLogLength+1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }


  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}


int loadModelA(vector<float> &vecVols, vector<float> &vecVals, GLuint VertexArrayIDA){
	int size_draw = 2*3*3*(N-1)+3*3;
  GLfloat g_vertex_buffer_dataA[size_draw];//+4*3*3];
  GLfloat g_vertex_normal_dataA[size_draw];//+4*3*3];
  GLfloat g_vertex_color_dataA[size_draw];

  for(int i=0; i<size_draw; i++){
    g_vertex_buffer_dataA[i] = 0.654321;
    g_vertex_color_dataA[i]  = 0.654321;
	g_vertex_normal_dataA[i] = 0.0f;
  }
  float couleur = 0;
  for (int i=0; i<N-1; i++){
	if (vecVals[i] < 0.5)
		couleur = 0.0f;
	else if (vecVals[i] < 0.8)
		couleur = 0.5f;
	else 
		couleur = 1.0f;
	float distance = i/2000.0;
float distance1 = (i+1)/2000.0;

	cout << distance<< endl;
	g_vertex_buffer_dataA[18*i + 0] = distance;
	g_vertex_buffer_dataA[18*i + 1] = 0.0f+profondeur_A;
	g_vertex_buffer_dataA[18*i + 2] = vecVals[i];

	g_vertex_buffer_dataA[18*i + 3] = distance;
	g_vertex_buffer_dataA[18*i + 4] = 1.0f+profondeur_A;
	g_vertex_buffer_dataA[18*i + 5] = vecVals[i];

	g_vertex_buffer_dataA[18*i + 6] = distance1;
	g_vertex_buffer_dataA[18*i + 7] = 0.0f+profondeur_A;
	g_vertex_buffer_dataA[18*i + 8] = vecVals[i+1];

	g_vertex_buffer_dataA[18*i + 9] = distance;
	g_vertex_buffer_dataA[18*i + 10] = 1.0f+profondeur_A;
	g_vertex_buffer_dataA[18*i + 11] = vecVals[i];

	g_vertex_buffer_dataA[18*i + 12] = distance1;
	g_vertex_buffer_dataA[18*i + 13] = 0.0f+profondeur_A;
	g_vertex_buffer_dataA[18*i + 14] = vecVals[i+1];

	g_vertex_buffer_dataA[18*i + 15] = distance1;
	g_vertex_buffer_dataA[18*i + 16] = 1.0f+profondeur_A;
	g_vertex_buffer_dataA[18*i + 17] = vecVals[i+1];

	g_vertex_color_dataA[18*i + 0] = couleur;
	g_vertex_color_dataA[18*i + 1] = 0.0f;
	g_vertex_color_dataA[18*i + 2] = 0.0f;

	g_vertex_color_dataA[18*i + 3] = couleur;
	g_vertex_color_dataA[18*i + 4] = 0.0f;
	g_vertex_color_dataA[18*i + 5] = 0.0f;

	g_vertex_color_dataA[18*i + 6] = couleur;
	g_vertex_color_dataA[18*i + 7] = 0.0f;
	g_vertex_color_dataA[18*i + 8] = 0.0f;

	g_vertex_color_dataA[18*i + 9] = couleur;
	g_vertex_color_dataA[18*i + 10] = 0.0f;
	g_vertex_color_dataA[18*i + 11] = 0.0f;

	g_vertex_color_dataA[18*i + 12] = couleur;
	g_vertex_color_dataA[18*i + 13] = 0.0f;
	g_vertex_color_dataA[18*i + 14] = 0.0f;

	g_vertex_color_dataA[18*i + 15] = couleur;
	g_vertex_color_dataA[18*i + 16] = 0.0f;
	g_vertex_color_dataA[18*i + 17] = 0.0f;

  }
g_vertex_buffer_dataA[size_draw-3*3] = 0.0f;
g_vertex_buffer_dataA[size_draw-3*3+1] = 0.0f;
g_vertex_buffer_dataA[size_draw-3*3+2] = 0.0f;

g_vertex_buffer_dataA[size_draw-3*3+3] = 1.0f;
g_vertex_buffer_dataA[size_draw-3*3+4] = 1.0f;
g_vertex_buffer_dataA[size_draw-3*3+5] = 0.0f;

g_vertex_buffer_dataA[size_draw-3*3+6] = 1.0f;
g_vertex_buffer_dataA[size_draw-3*3+7] = 0.0f;
g_vertex_buffer_dataA[size_draw-3*3+8] = 0.0f;


g_vertex_color_dataA[size_draw-3*3] = 0.0f;
g_vertex_color_dataA[size_draw-3*3+1] = 0.0f;
g_vertex_color_dataA[size_draw-3*3+2] = 1.0f;

g_vertex_color_dataA[size_draw-3*3+3] = 0.0f;
g_vertex_color_dataA[size_draw-3*3+4] = 0.0f;
g_vertex_color_dataA[size_draw-3*3+5] = 1.0f;

g_vertex_color_dataA[size_draw-3*3+6] = 0.0f;
g_vertex_color_dataA[size_draw-3*3+7] = 0.0f;
g_vertex_color_dataA[size_draw-3*3+8] = 1.0f;
/**
	g_vertex_buffer_dataA[size_draw+0] = 0.0;
    g_vertex_buffer_dataA[size_draw+1] = vecVals[0];
    g_vertex_buffer_dataA[size_draw+2] = 0.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+3] = 0.0;
    g_vertex_buffer_dataA[size_draw+4] = 0;
    g_vertex_buffer_dataA[size_draw+5] = 0.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+6] = 0.0;
    g_vertex_buffer_dataA[size_draw+7] = 0;
    g_vertex_buffer_dataA[size_draw+8] = 1.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+9] = 0.0;
    g_vertex_buffer_dataA[size_draw+10] = vecVals[0];
    g_vertex_buffer_dataA[size_draw+11] = 0.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+12] = 0.0;
    g_vertex_buffer_dataA[size_draw+13] = 0;
    g_vertex_buffer_dataA[size_draw+14] = 1.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+15] = 0.0;
    g_vertex_buffer_dataA[size_draw+16] = vecVals[size_draw-1];
    g_vertex_buffer_dataA[size_draw+17] = 1.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+18] = N-1;
    g_vertex_buffer_dataA[size_draw+19] = vecVals[size_draw-1];
    g_vertex_buffer_dataA[size_draw+20] = 0.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+21] = N-1;
    g_vertex_buffer_dataA[size_draw+22] = 0;
    g_vertex_buffer_dataA[size_draw+23] = 0.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+24] = N-1;
    g_vertex_buffer_dataA[size_draw+25] = 0;
    g_vertex_buffer_dataA[size_draw+26] = 1.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+27] = N-1;
    g_vertex_buffer_dataA[size_draw+28] = vecVals[N-1];
    g_vertex_buffer_dataA[size_draw+29] = 0.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+30] = N-1;
    g_vertex_buffer_dataA[size_draw+31] = 0;
    g_vertex_buffer_dataA[size_draw+32] = 1.0+profondeur_A;

    g_vertex_buffer_dataA[size_draw+33] = N-1;
    g_vertex_buffer_dataA[size_draw+34] = vecVals[N-1];
    g_vertex_buffer_dataA[size_draw+35] = 1.0+profondeur_A;

	g_vertex_color_dataA[size_draw+0] = 0.0f;
    g_vertex_color_dataA[size_draw+1] = 1.0f;
    g_vertex_color_dataA[size_draw+2] = 0.0f;

    g_vertex_color_dataA[size_draw+3] = 0.0f;
    g_vertex_color_dataA[size_draw+4] = 1.0f;
    g_vertex_color_dataA[size_draw+5] = 0.0f;

    g_vertex_color_dataA[size_draw+6] = 0.0f;
    g_vertex_color_dataA[size_draw+7] = 1.0f;
    g_vertex_color_dataA[size_draw+8] = 0.0f;

    g_vertex_color_dataA[size_draw+9] = 0.0f;
    g_vertex_color_dataA[size_draw+10] = 1.0f;
    g_vertex_color_dataA[size_draw+11] = 0.0f;

    g_vertex_color_dataA[size_draw+12] = 0.0f;
    g_vertex_color_dataA[size_draw+13] = 1.0f;
    g_vertex_color_dataA[size_draw+14] = 0.0f;

    g_vertex_color_dataA[size_draw+15] = 0.0f;
    g_vertex_color_dataA[size_draw+16] = 1.0f;
    g_vertex_color_dataA[size_draw+17] = 0.0f;

    g_vertex_color_dataA[size_draw+18] = 0.0f;
    g_vertex_color_dataA[size_draw+19] = 1.0f;
    g_vertex_color_dataA[size_draw+20] = 0.0f;

    g_vertex_color_dataA[size_draw+21] = 0.0f;
    g_vertex_color_dataA[size_draw+22] = 1.0f;
    g_vertex_color_dataA[size_draw+23] = 0.0f;

    g_vertex_color_dataA[size_draw+24] = 0.0f;
    g_vertex_color_dataA[size_draw+25] = 1.0f;
    g_vertex_color_dataA[size_draw+26] = 0.0f;

    g_vertex_color_dataA[size_draw+27] = 0.0f;
    g_vertex_color_dataA[size_draw+28] = 1.0f;
    g_vertex_color_dataA[size_draw+29] = 0.0f;

    g_vertex_color_dataA[size_draw+30] = 0.0f;
    g_vertex_color_dataA[size_draw+31] = 1.0f;
    g_vertex_color_dataA[size_draw+32] = 0.0f;

    g_vertex_color_dataA[size_draw+33] = 0.0f;
    g_vertex_color_dataA[size_draw+34] = 1.0f;
    g_vertex_color_dataA[size_draw+35] = 0.0f;
**/
	profondeur_A+=2;

  // on teste s'il ne reste pas notre valeur bizarre dans le tableau = on a pas oublie de cases !
  for(int i=0; i<size_draw; i++)
    if (g_vertex_buffer_dataA[i] > 0.654320 && g_vertex_buffer_dataA[i] < 0.654322)
      cout << i<<" EVIL IS IN THE DETAIL !" << endl ;

  glBindVertexArray(VertexArrayIDA);

  // This will identify our vertex buffer
  GLuint vertexbufferA;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbufferA);

  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferA);

    // Only allocate memory. Do not send yet our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_dataA)+sizeof(g_vertex_color_dataA)+sizeof(g_vertex_normal_dataA), 0, GL_STATIC_DRAW);

      // send vertices in the first part of the buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0,                            sizeof(g_vertex_buffer_dataA), g_vertex_buffer_dataA);

    // send colors in the second part of the buffer
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_dataA), sizeof(g_vertex_color_dataA), g_vertex_color_dataA);

    // send bormals in the third part of the buffer
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_dataA)+sizeof(g_vertex_color_dataA), sizeof(g_vertex_normal_dataA), g_vertex_normal_dataA);

    // ici les commandes stockees "une fois pour toute" dans le VAO
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer( // same thing for the colors
      1,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)sizeof(g_vertex_buffer_dataA));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer( // same thing for the normals
      2,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)(sizeof(g_vertex_buffer_dataA)+sizeof(g_vertex_color_dataA)));
    glEnableVertexAttribArray(2);


  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // on desactive le VAO a la fin de l'initialisation
  glBindVertexArray (0);
	cout << (size_draw)/3 << "   " << sizeof(g_vertex_buffer_dataA)/(3*sizeof(float)) << endl;
  return sizeof(g_vertex_buffer_dataA)/(3*sizeof(float));
}



int loadModelB(vector<float> &vecVols, vector<float> &vecVals, GLuint VertexArrayIDB){
	int size_draw = 4*3*3*(N-1);
  GLfloat g_vertex_buffer_dataB[size_draw];
	GLfloat g_vertex_color_dataB[size_draw]; // 4 triangles of 3 points with 3 coordinates each

  //cout<< "loadModelB " << (sizeof(g_vertex_buffer_dataB)/(sizeof(float)))<<" % "<<(3*3*4*(N))<<endl;
  for(int i=0; i<size_draw; i++){
    g_vertex_buffer_dataB[i] = 0.654321;
	g_vertex_color_dataB[i] = 0.654321;
}

  // on rajoute des faces et de la hauteur a notre figure
  for (int i=0; i<N-1; i++){
	float distance = i/2000.0;
float distance1 = (i+1)/2000.0;
    g_vertex_buffer_dataB[36*i + 0] = distance;
    g_vertex_buffer_dataB[36*i + 1] = 0.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 2] = vecVals[i];

    g_vertex_buffer_dataB[36*i + 3] = distance;
    g_vertex_buffer_dataB[36*i + 4] = 0.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 5] = 0.0f;

    g_vertex_buffer_dataB[36*i + 6] = distance1;
    g_vertex_buffer_dataB[36*i + 7] = 0.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 8] = 0.0f;

    g_vertex_buffer_dataB[36*i + 9] = distance;
    g_vertex_buffer_dataB[36*i + 10] = 0.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 11] = vecVals[i];

    g_vertex_buffer_dataB[36*i + 12] = distance1;
    g_vertex_buffer_dataB[36*i + 13] = 0.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 14] = vecVals[i+1];

    g_vertex_buffer_dataB[36*i + 15] = distance1;
    g_vertex_buffer_dataB[36*i + 16] = 0.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 17] = 0.0f;

    g_vertex_buffer_dataB[36*i + 18] = distance;
    g_vertex_buffer_dataB[36*i + 19] = 1.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 20] = vecVals[i];

    g_vertex_buffer_dataB[36*i + 21] = distance;
    g_vertex_buffer_dataB[36*i + 22] = 1.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 23] = 0.0f;

    g_vertex_buffer_dataB[36*i + 24] = distance1;
    g_vertex_buffer_dataB[36*i + 25] = 1.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 26] = 0.0f;

    g_vertex_buffer_dataB[36*i + 27] = distance;
    g_vertex_buffer_dataB[36*i + 28] = 1.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 29] = vecVals[i];

    g_vertex_buffer_dataB[36*i + 30] = distance1;
    g_vertex_buffer_dataB[36*i + 31] = 1.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 32] = vecVals[i+1];

    g_vertex_buffer_dataB[36*i + 33] = distance1;
    g_vertex_buffer_dataB[36*i + 34] = 1.0f+profondeur_B;
    g_vertex_buffer_dataB[36*i + 35] = 0.0f;
	cout << vecVals[i] << endl;
  }

profondeur_B+=2;
  // ???
  
  for(int i=0; i<size_draw; i++)
    if (g_vertex_buffer_dataB[i] > 0.654320 && g_vertex_buffer_dataB[i] < 0.654322)
      cout << i<<" EVIL IS IN THE DETAIL !" << endl ;

  // This will identify our vertex buffer
  GLuint vertexbufferB;

  // on s'occupe du second VAO
  glBindVertexArray(VertexArrayIDB);
  glGenBuffers(1, &vertexbufferB);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_dataB), 0, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0,                            sizeof(g_vertex_buffer_dataB), g_vertex_buffer_dataB);
  glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
  glEnableVertexAttribArray(0);


  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // on desactive le VAO a la fin de l'initialisation
  glBindVertexArray (0);
  return sizeof(g_vertex_buffer_dataB)/(3*sizeof(float));
}


int main(){
  // for later printing
  std::cout << std::setprecision(5) << std::fixed;

  vector<float> kernel0 = computeGaussianKernel(SMOOTHING_VALS);

  for (int i = 0; i < kernel0.size(); i++)
    cout << i<<": " << kernel0[i] << endl;

  // Initialise GLFW
  if( !glfwInit() ) {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      return -1;
  }

  vector<float> vols10;
  vector<float> vals10;

  vector<float> vols20;
  vector<float> vals20;

  vector<float> vols30;
  vector<float> vals30;

  vector<float> vols40;
  vector<float> vals40;

  loadData("data/AAPL.csv",  vols10, vals10);
  loadData("data/GOOGL.csv", vols20, vals20);
  loadData("data/AMZN.csv",  vols30, vals30);
  loadData("data/MSFT.csv",  vols40, vals40);
  N = vals10.size()-3;

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // On veut OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Pour rendre MacOS heureux ; ne devrait pas être nécessaire
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // On ne veut pas l'ancien OpenGL
  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  // Ouvre une fenêtre et crée son contexte OpenGl
  GLFWwindow* window; // (Dans le code source qui accompagne, cette variable est globale)
  window = glfwCreateWindow( 1600, 768, "Projet 2018", NULL, NULL);
  if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
      glfwTerminate();
      return -1;
  }

  glfwMakeContextCurrent(window); // Initialise GLEW
  glewExperimental=true; // Nécessaire dans le profil de base
  if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      return -1;
  }

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  glDepthRange(-1, 1);

  // Assure que l'on peut capturer la touche d'échappement enfoncée ci-dessous
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Bon maintenant on cree les IDs de VAO
  GLuint VertexArrayIDA1, VertexArrayIDB1;
  GLuint VertexArrayIDA2, VertexArrayIDB2;
  GLuint VertexArrayIDA3, VertexArrayIDB3;
  GLuint VertexArrayIDA4, VertexArrayIDB4;

  glGenVertexArrays(1, &VertexArrayIDA1);
  glGenVertexArrays(1, &VertexArrayIDB1);
  glGenVertexArrays(1, &VertexArrayIDA2);
  glGenVertexArrays(1, &VertexArrayIDB2);
  glGenVertexArrays(1, &VertexArrayIDA3);
  glGenVertexArrays(1, &VertexArrayIDB3);
  glGenVertexArrays(1, &VertexArrayIDA4);
  glGenVertexArrays(1, &VertexArrayIDB4);

  int m11 = loadModelA(vols10, vals10, VertexArrayIDA1);
  int m12 = loadModelB(vols10, vals10, VertexArrayIDB1);
 
  int m21 = loadModelA(vols20, vals20, VertexArrayIDA2);
  int m22 = loadModelB(vols20, vals20, VertexArrayIDB2);

  int m31 = loadModelA(vols30, vals30, VertexArrayIDA3);
  int m32 = loadModelB(vols30, vals30, VertexArrayIDB3);

  int m41 = loadModelA(vols40, vals40, VertexArrayIDA4);
  int m42 = loadModelB(vols40, vals40, VertexArrayIDB4);

  GLuint ProgramA        = LoadShaders( "projet2018A.vs", "projet2018A.fs" );
  GLint  uniform_projection     = glGetUniformLocation(ProgramA, "projectionMatrix");
  GLint  uniform_view     = glGetUniformLocation(ProgramA, "viewMatrix");
  GLint uniform_modelA	= glGetUniformLocation(ProgramA, "modelMatrixA");

  GLuint ProgramB        = LoadShaders( "projet2018B.vs", "projet2018B.fs" );
GLint uniform_modelB	= glGetUniformLocation(ProgramB, "modelMatrixB");

	
	double angle = 0.0;
	float angleRotate_X = 0;
	float angleRotate_Y = 0;
	float angleRotate_Z = 0;
//float camPos[3] = {5+cos(angle), 5+sin(angle), -0.5};
  do {
    // clear before every draw
    glClearColor( 1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader program
    glUseProgram(ProgramA);
	//angle += 0.1;
    // onchange de matrice de projection : la projection orthogonale est plus propice a la visualization !
    //glm::mat4 projectionMatrix = glm::perspective(glm::radians(66.0f), 1024.0f / 768.0f, 0.1f, 200.0f);
    glm::mat4 projectionMatrix = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f, -6.f, 6.f );
    float camPos[3] = {5*cos(angle), 5*sin(angle), -.5};
    glm::mat4 viewMatrix       = glm::lookAt(
                                  glm::make_vec3(camPos), // where is the camara
                                  vec3(0, 0, 0), //where it looks
                                  vec3(0, 0, 1) // head is up
                                );
    mat4 rotation_X = glm::mat4(1.0f); //Cree la matrice identite
	rotation_X[1][1] = cos(angleRotate_X);
	rotation_X[1][2] = -sin(angleRotate_X);
	rotation_X[2][1] = sin(angleRotate_X);
	rotation_X[2][2] = cos(angleRotate_X);
mat4 rotation_Y = glm::mat4(1.0f); //Cree la matrice identite
	rotation_Y[0][0] = cos(angleRotate_Y);
	rotation_Y[0][2] = sin(angleRotate_Y);
	rotation_Y[2][0] = -sin(angleRotate_Y);
	rotation_Y[2][2] = cos(angleRotate_Y);
mat4 rotation_Z = glm::mat4(1.0f); //Cree la matrice identite
	rotation_Z[0][0] = cos(angleRotate_Z);
	rotation_Z[0][1] = -sin(angleRotate_Z);
	rotation_Z[1][0] = sin(angleRotate_Z);
	rotation_Z[1][1] = cos(angleRotate_Z);
    mat4 modelMatrixA     =  scale(glm::mat4(1.0f), glm::vec3(0.75f));
    modelMatrixA          =  translate(modelMatrixA, glm::vec3(0.0f, 0.3f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(0.75f));
mat4 modelMatrixB     =  scale(glm::mat4(1.0f), glm::vec3(0.75f));
    modelMatrixB          =  translate(modelMatrixB, glm::vec3(0.0f, 0.3f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(0.75f)) ;
	


    // on envoie les valeurs uniforme aux shaders
    glUniformMatrix4fv(uniform_view,  1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniform_projection,1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniform_modelA,1, GL_FALSE, glm::value_ptr(modelMatrixA));

    // on re-active les VAO avant d'envoyer les buffers
    glBindVertexArray(VertexArrayIDA1);
    glDrawArrays(GL_TRIANGLES, 0, m11);
/**
    glBindVertexArray(VertexArrayIDA2);
    glDrawArrays(GL_TRIANGLES, 0, m21);

    glBindVertexArray(VertexArrayIDA3);
    glDrawArrays(GL_TRIANGLES, 0, m31);

    glBindVertexArray(VertexArrayIDA4);
    glDrawArrays(GL_TRIANGLES, 0, m41);
**/

    glUseProgram(ProgramB);
    glUniformMatrix4fv(uniform_view,  1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniform_projection,1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniform_modelB,1, GL_FALSE, glm::value_ptr(modelMatrixB));

    glBindVertexArray(VertexArrayIDB1);
    glDrawArrays(GL_TRIANGLES, 0, m12); // Starting from vertex 0 .. all the buffer
/**
    glBindVertexArray(VertexArrayIDB2);
    glDrawArrays(GL_TRIANGLES, 0, m22);

    glBindVertexArray(VertexArrayIDB3);
    glDrawArrays(GL_TRIANGLES, 0, m32);

    glBindVertexArray(VertexArrayIDB4);
    glDrawArrays(GL_TRIANGLES, 0, m42);
**/
    // on desactive le VAO a la fin du dessin
    glBindVertexArray (0);

    // on desactive les shaders
    glUseProgram(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
	//std::cout << "X = "<< camPos[0] << "; Y = " << camPos[1] << "; Z = " << camPos[2] << endl;
    if (glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS){
      angle+=0.01;
    } else if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
      angle -=0.01;
    } else if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS){
      camPos[0] = camPos[0];
      camPos[1] = camPos[1];
      camPos[2] = camPos[2]+0.01;
	  //glRotatef(angleRotate,0,1,0);
    } else if (glfwGetKey(window, GLFW_KEY_F ) == GLFW_PRESS){
      camPos[0] = camPos[0];
      camPos[1] = camPos[1];
      camPos[2] = camPos[2]-0.01;
    } else if (glfwGetKey(window, GLFW_KEY_P ) == GLFW_PRESS){
	angleRotate_Y +=0.01;
    } else if (glfwGetKey(window, GLFW_KEY_N ) == GLFW_PRESS){
	angleRotate_Y -=0.01;
    } else if (glfwGetKey(window, GLFW_KEY_O ) == GLFW_PRESS){
	angleRotate_Z += 0.01;
    } else if (glfwGetKey(window, GLFW_KEY_L ) == GLFW_PRESS) {
	angleRotate_Z -= 0.01;
    } else if (glfwGetKey(window, GLFW_KEY_I ) == GLFW_PRESS) {
	angleRotate_X += 0.01;
    } else if (glfwGetKey(window, GLFW_KEY_K ) == GLFW_PRESS) {
	angleRotate_X -= 0.01;
    } else if ( glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS ){
      camPos[0] = camPos[0]-0.01;
	  camPos[1] = camPos[1];
	  camPos[2] = camPos[2];
    } else if ( glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS ){
//&& SMOOTHING_VALS<32){
      camPos[0] = camPos[0]+0.01;
	  camPos[1] = camPos[1];
	  camPos[2] = camPos[2];
    } else if ( glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS ){
//&& SMOOTHING_VOLS>1){
      camPos[0] = camPos[0];
	  camPos[1] = camPos[1];
	  camPos[2] = camPos[2]+0.1;
    } else if ( glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS ){//&& SMOOTHING_VOLS<32){
      camPos[0] = camPos[0];
	  camPos[1] = camPos[1];
	  camPos[2] = camPos[2]-0.1;
    }

  } // Vérifie si on a appuyé sur la touche échap (ESC) ou si la fenêtre a été fermée
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
}
